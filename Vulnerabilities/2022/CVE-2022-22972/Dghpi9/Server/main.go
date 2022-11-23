package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
)

func FuckVMware(w http.ResponseWriter, r *http.Request) {
	w.WriteHeader(200)
	io.WriteString(w, "ok\n")
	_, _ = io.Copy(os.Stdout, r.Body)
}

func main() {
	http.HandleFunc("/", FuckVMware)
	//if err := http.ListenAndServeTLS(":8443", "cert.pem", "key.pem", nil); err != nil {
	//	fmt.Printf("http service failed err:%v\n", err)
	//}
	if err := http.ListenAndServe(":2334", nil); err != nil {
		fmt.Printf("http service failed err:%v\n", err)
	}
}
