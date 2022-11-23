package main

import (
	"crypto/rand"
	"fmt"
)

func GenerateRandomName(n int) string {
	letters := []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")

	random := make([]byte, n)
	_, err := rand.Read(random)
	AssertOk(err)

	out := make([]rune, n)
	for idx, b := range random {
		out[idx] = letters[b%byte(len(letters))]
	}
	return string(out)
}

func AssertOk(err error, messages ...string) {
	if err != nil {
		fmt.Printf("Assertion failed\n")
		for msg := range messages {
			fmt.Println(msg)
		}
		panic(err)
	}
}
