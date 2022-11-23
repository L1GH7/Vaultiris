package main

import (
	"flag"
	"fmt"
	"log"
	"strings"
	"time"
)

func main() {
	redirectorAddress := flag.String("redirectorAddress", "172.16.146.1:8081", "redirector server address")
	targetHost := flag.String("targetHost", "127.0.0.1", "targetHost address")
	targetPorts := flag.String("targetPorts", "80,443,22", "comma separated target ports")
	serverRoot := flag.String("serverRoot", "http://172.16.66.130", "open xchange server root url")
	serverUser := flag.String("username", "testuser", "open xchange server username")
	serverPass := flag.String("password", "secret", "open xchange server password")
	numSamples := flag.Int("numSamples", 1, "maximum number of samples")
	flag.Parse()

	log.Printf(*targetHost)

	store := NewInMemoryRedirectorStore()
	go startRedirector(*redirectorAddress, store)

	config := XCConfig{
		Root:     *serverRoot,
		Username: *serverUser,
		Password: *serverPass,
	}
	client := NewXCClient(config)
	AssertOk(client.Login())
	defer func() { AssertOk(client.Logout()) }()

	ports := strings.Split(*targetPorts, ",")

	result := make(map[string]float64)

	maxSampleDuration := 5 * time.Second
	var i = 0
	for _, port := range ports {
		durationSum := time.Second * 0
		sampleStartTime := time.Now()
		for i = 0; i < *numSamples; i++ {
			sessionId := GenerateRandomName(SessionIdLength)
			session := NewRedirectSession("http://" + *targetHost + ":" + port + "/image.png")
			AssertOk(store.Set(sessionId, session))

			url := fmt.Sprintf("http://%s/redirect?session=%s", *redirectorAddress, sessionId)
			res, err := client.DocAddFile(url)
			if err == nil {
				panic(fmt.Sprintf("exptected error, got result %v", res))
			}
			log.Printf("got error %v", err)

			endTime := time.Now()
			startTime, err := session.RedirectTime()
			AssertOk(err)
			durationSum += endTime.Sub(startTime)

			if time.Now().Sub(sampleStartTime) > maxSampleDuration {
				break
			}
		}
		result[port] = float64(durationSum.Milliseconds()) / float64(i)
	}

	for _, port := range ports {
		log.Printf("%s: %f", port, result[port])
	}
}
