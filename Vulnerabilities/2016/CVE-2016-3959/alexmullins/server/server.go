// Thanks to jpillora for the example server to test out
// https://gist.github.com/jpillora/b480fde82bff51a06238
// Modified a bit to accept public key auth and return the current
// time when a new session is created.
//
// A small SSH daemon providing bash sessions
//
// Server:
// cd my/new/dir/
// #generate server keypair
// ssh-keygen -t rsa
// go get -v .
// go run sshd.go
//
// Client:
// ssh foo@localhost -p 2200 #pass=bar

package main

import (
	"bytes"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net"
	"os/exec"
	"strings"

	"golang.org/x/crypto/ssh"
)

var (
	addr = flag.String("address", "localhost:8022", "ssh server address")
	key  = flag.String("key", "", "host key")
	p    = flag.Bool("p", false, "parallel tcp accept")
)

func init() {
	flag.Parse()
	if *key == "" {
		log.Fatalln("must provide a host key.")
	}
	*addr = strings.TrimSpace(*addr)
	*key = strings.TrimSpace(*key)
}

func main() {

	// In the latest version of crypto/ssh (after Go 1.3), the SSH server type has been removed
	// in favour of an SSH connection type. A ssh.ServerConn is created by passing an existing
	// net.Conn and a ssh.ServerConfig to ssh.NewServerConn, in effect, upgrading the net.Conn
	// into an ssh.ServerConn

	config := &ssh.ServerConfig{
		// Accept all authentication requests
		PublicKeyCallback: func(c ssh.ConnMetadata, key ssh.PublicKey) (*ssh.Permissions, error) {
			return nil, nil
		},
	}

	// You can generate a keypair with 'ssh-keygen -t rsa'
	privateBytes, err := ioutil.ReadFile(*key)
	if err != nil {
		log.Fatal("Failed to load private key: ", *key)
	}

	private, err := ssh.ParsePrivateKey(privateBytes)
	if err != nil {
		log.Fatal("Failed to parse private key")
	}

	config.AddHostKey(private)

	// Once a ServerConfig has been configured, connections can be accepted.
	listener, err := net.Listen("tcp", *addr)
	if err != nil {
		log.Fatalf("Failed to listen on %s: %s", *addr, err)
	}

	// Accept all connections
	log.Println("Listening on", *addr)
	for {
		tcpConn, err := listener.Accept()
		if err != nil {
			log.Printf("Failed to accept incoming connection (%s)", err)
			return
		}
		log.Printf("Accepted an incoming TCP connection from %s", tcpConn.RemoteAddr())
		if *p {
			go makeSSHConn(tcpConn, config)
		} else {
			makeSSHConn(tcpConn, config)
		}

	}
}

func makeSSHConn(conn net.Conn, config *ssh.ServerConfig) {
	// Before use, a handshake must be performed on the incoming net.Conn.
	sshConn, chans, reqs, err := ssh.NewServerConn(conn, config)
	if err != nil {
		log.Printf("Failed to handshake (%s)", err)
		return
	}

	log.Printf("New SSH connection from %s (%s)", sshConn.RemoteAddr(), sshConn.ClientVersion())
	// Discard all global out-of-band Requests
	go ssh.DiscardRequests(reqs)
	// Accept all channels
	go handleChannels(chans)
}

func handleChannels(chans <-chan ssh.NewChannel) {
	// Service the incoming Channel channel in go routine
	for newChannel := range chans {
		go handleChannel(newChannel)
	}
}

func handleChannel(newChannel ssh.NewChannel) {
	// Since we're handling a shell, we expect a
	// channel type of "session".
	if t := newChannel.ChannelType(); t != "session" {
		newChannel.Reject(ssh.Prohibited, fmt.Sprintf("was not a session: %s", t))
		return
	}

	// At this point, we have the opportunity to reject the client's
	// request for another logical connection
	chConn, requests, err := newChannel.Accept()
	if err != nil {
		log.Printf("Could not accept channel (%s)", err)
		return
	}
	defer chConn.Close()
	done := make(chan bool)
	defer close(done)

	go func(done chan bool, reqs <-chan *ssh.Request) {
		for {
			select {
			case <-done:
				return
			case req := <-reqs:
				req.Reply(false, nil)
			}
		}
	}(done, requests)

	cmd := exec.Command("date")
	output, err := cmd.Output()
	if err != nil {
		log.Println("failed to run date cmd")
		return
	}
	if _, err = io.Copy(chConn, bytes.NewReader(output)); err != nil {
		log.Println("failed to copy data")
		return
	}
}
