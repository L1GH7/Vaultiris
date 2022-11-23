package main

import (
	"crypto/dsa"
	"crypto/rand"
	"crypto/sha1"
	"flag"
	"math/big"
	"testing"

	"golang.org/x/crypto/ssh"
)

var (
	fail = flag.Bool("fail", false, "panic dsa")
)

func init() {
	flag.Parse()
}

func generatePrivKey(t *testing.T) *dsa.PrivateKey {
	// Create the DSA parameters
	params := dsa.Parameters{}
	err := dsa.GenerateParameters(&params, rand.Reader, dsa.L1024N160)
	if err != nil {
		t.Fatalf("failed to generate dsa parameters: %v", err)
	}

	// Create the DSA private/public keys
	priv := new(dsa.PrivateKey)
	priv.Parameters = params
	err = dsa.GenerateKey(priv, rand.Reader)
	if err != nil {
		t.Fatalf("failed to generate dsa parameters: %v", err)
	}
	return priv
}

func TestDSASignature(t *testing.T) {
	var message = "Hello brave new world!"
	var hash = sha1.Sum([]byte(message))
	var err error

	priv := generatePrivKey(t)

	// Sign a message
	r, s, err := dsa.Sign(rand.Reader, priv, hash[:])
	if err != nil {
		t.Fatalf("failed to sign message: %v", err)
	}

	if !dsa.Verify(&priv.PublicKey, hash[:], r, s) {
		t.Fatalf("failed to verify message: %v", err)
	}
}

func TestDSAPanic(t *testing.T) {
	flag.Parse()
	if !*fail {
		t.Skip()
	}

	var message = "Hello brave new world!"
	var hash = sha1.Sum([]byte(message))
	var err error

	priv := generatePrivKey(t)

	// Sign a message
	r, s, err := dsa.Sign(rand.Reader, priv, hash[:])
	if err != nil {
		t.Fatalf("failed to sign message: %v", err)
	}

	priv.P = new(big.Int).SetInt64(0)

	if !dsa.Verify(&priv.PublicKey, hash[:], r, s) {
		t.Fatalf("failed to verify message: %v", err)
	}
}

func TestSSHUnmarshal(t *testing.T) {
	b := []byte{0, 0, 0, 4, 100, 97, 116, 101}
	type cmdReq struct {
		Cmd string
	}
	cmd := new(cmdReq)
	err := ssh.Unmarshal(b, cmd)
	if err != nil {
		t.Fatal("failed to parse cmd")
	}
	if cmd.Cmd != "date" {
		t.Fatal("did not get date: ", cmd.Cmd)
	}
}
