package DataHandle

import (
	"flag"
	"fmt"
	"os"
)

var (
	h          bool
	TargetFile string
	Target     string
	Thread     int
)

func init() {
	fmt.Println(`  
    ___    ____  _________ _____  __    __________  ___   ________ __
   /   |  / __ \/  _/ ___//  _/ |/ /   / ____/ __ \/   | / ____/ //_/
  / /| | / /_/ // / \__ \ / / |   /   / /   / /_/ / /| |/ /   / ,<
 / ___ |/ ____// / ___/ // / /   |   / /___/ _, _/ ___ / /___/ /| |
/_/  |_/_/   /___//____/___//_/|_|   \____/_/ |_/_/  |_\____/_/ |_| `, "\n\n\t\t\t\t\t\t\t\t\tBY: 玉兔开源漏洞工具实践项目")
	fmt.Println("\n------------------------------------------------------------------------------\n1、Apache APISIX 未授权漏洞（CVE-2021-45232）\n\n2、Apache APISIX 默认密钥漏洞(CVE-2020-13945)\n\n------------------------------------------------------------------------------\n")
	flag.StringVar(&TargetFile, "TF", "", "批量目标：-TF url.txt -t 500")
	flag.StringVar(&Target, "T", "", "单个目标：-T http://127.0.0.1")
	flag.IntVar(&Thread, "t", 500, "并发数量")
	flag.BoolVar(&h, "h", false, "Help")

	// 修改提示信息

	flag.Usage = usage
	flag.Parse()
	if h || ((TargetFile == "") && (Target == "")) {
		flag.Usage()
		os.Exit(0)
	}
}

func usage() {
	fmt.Fprintf(os.Stderr, "Usage:\n\n")
	flag.PrintDefaults()

}
