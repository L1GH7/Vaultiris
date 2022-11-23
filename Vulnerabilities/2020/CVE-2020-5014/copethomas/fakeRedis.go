package main

import (
	"io"
	"io/ioutil"
	"log"
	"net"
	"os"
	"strconv"
	"strings"
)

var (
	Logger *log.Logger
	done   bool
)

func FakeRedis(host string, port int, c chan bool, module string, shell chan net.Conn) {
	Logger = log.New(os.Stdout, "FakeRedis - ", log.Ldate|log.Ltime)
	Logger.Printf("Starting Fake Redis Server on %s:%d", host, port)
	l, err := net.Listen("tcp", host+":"+strconv.Itoa(port))
	if err != nil {
		Logger.Fatal("Error listening:", err.Error())
	}
	defer l.Close()
	Logger.Printf("Online and Ready!")
	for {
		conn, err := l.Accept()
		Logger.Printf("Accepting connection...")
		if err != nil {
			Logger.Fatal("Error accepting: ", err.Error())
		}
		if done {
			shell <- conn
		} else {
			go handleRequest(conn, c, module)
		}
	}
}

func sendRawData(conn net.Conn, data string) {
	_, err := io.WriteString(conn, data)
	if err != nil {
		Logger.Fatal("Error sending data: ", err.Error())
	}
}

func sendData(conn net.Conn, data string) {
	sendRawData(conn, data)
	//Logger.Printf("Data : DatapowerRedis <- FakeRedis = %s", data)
	Logger.Printf("Data : DatapowerRedis <- FakeRedis")
	sendRawData(conn, "\r\n")
}

func handleRequest(conn net.Conn, c chan bool, module string) {
	Logger.Printf("Accepted Connection OK!")
	for {
		buf := make([]byte, 1024)
		_, err := conn.Read(buf)
		if err != nil {
			if done {
				Logger.Printf("Error reading data from network connection: %s - (This is expected)", err.Error())
				break
			} else {
				Logger.Fatal("Error data from network connection :", err.Error())
			}
		}
		//Logger.Printf("Data : DatapowerRedis -> FakeRedis = %s", string(buf))
		Logger.Printf("Data : DatapowerRedis -> FakeRedis")
		if strings.Contains(string(buf), "PING") {
			sendData(conn, "+PONG")
		}
		if strings.Contains(string(buf), "REPLCONF") || strings.Contains(string(buf), "AUTH") {
			sendData(conn, "+OK")
		}
		if strings.Contains(string(buf), "PSYNC") || strings.Contains(string(buf), "SYNC") {
			Logger.Println("Uploading module...")
			sendRawData(conn, "+FULLRESYNC ")
			sendRawData(conn, strings.Repeat("Z", 40))
			sendRawData(conn, " 1")
			sendRawData(conn, "\r\n")
			sendRawData(conn, "$")
			fi, err := os.Stat(module)
			if err != nil {
				Logger.Fatal("Error getting size of module:", err.Error())
			}
			sendRawData(conn, strconv.Itoa(int(fi.Size())))
			sendRawData(conn, "\r\n")
			moduleRawData, err := ioutil.ReadFile(module)
			if err != nil {
				Logger.Fatal("failed to read module from disk: ", err.Error())
			}
			_, err = conn.Write(moduleRawData)
			if err != nil {
				Logger.Fatal("Error sending module raw data: ", err.Error())
			}
			sendRawData(conn, "\r\n")
			Logger.Println("Upload Complete!")
			c <- true
			done = true
		}
	}
	conn.Close()
	Logger.Printf("Connection Closed")
}
