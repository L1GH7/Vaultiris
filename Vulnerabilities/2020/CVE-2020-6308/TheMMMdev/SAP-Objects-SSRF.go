package main

import (
  
	"fmt"
	"time"
	"os"
	"net/http"
	"bytes"
	"strconv"
  "flag"
)


  func makeRequest (port string, url string, verbose bool) {
  	    
    start := time.Now()
  	  client := &http.Client{}
         parameters := ("aps=127.0.0.1:"+port+"&usr=admin&pwd=&aut=secEnterprise&main_page=ie.jsp&new_pass_page=newpwdform.jsp&exit_page=logonform.jsp")
         str := []byte(parameters)
         req, err := http.NewRequest("POST", url, bytes.NewBuffer(str))
   		 req.Header.Set("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")
  		 req.Header.Set("Accept-Language", "en-US,en;q=0.5")
		   req.Header.Set("Accept-Encoding", "gzip, deflate")
       req.Header.Set("Content-Type", "application/x-www-form-urlencoded")
  		 req.Header.Set("Cookie", "JSESSIONID=DD7DBFE4FEA8CE55AD662D2874FE6345; JSESSIONID=074D7A13D744DE8119FC4658022351F9; InfoViewPLATFORMSVC_COOKIE_TOKEN=")
       req.Header.Set("User-Agent", "Does not really matter")
    	 resp, err := client.Do(req)
    		if err != nil {
 		       panic(err)
 		   }
  		 defer resp.Body.Close()

         elapsed := time.Since(start).Seconds()
        if elapsed > 9 {
         fmt.Printf("Port open: %s. Response took %v seconds \n", port, elapsed)
        } else if elapsed < 9 && elapsed > 4 {
          fmt.Printf("Port closed, filtered, or firewalled: %s. Response took %v seconds \n", port, elapsed)
        } else if elapsed <= 4 && verbose {
           fmt.Printf("Port closed: %s. Response took %v seconds \n", port, elapsed)
        }
       
  }


 func main() {
        var CommandLine = flag.NewFlagSet(os.Args[0], flag.ExitOnError)
        var intSlice = []int{22, 25, 3389, 445, 9200, 6379, 135, 5000}
        var portLength = 65535
        var mediumList = []int{15,17,18,18,19,19,21,22,23,25,80,81,123,389,443,636,989,990,1433,1499,2022,2122,2222,2375,2376,2380,2480,2483,2484,2638,3000,3001,3020,3306,3389,3500,3999,4000,4100,4200,4243,4244,4444,4500,4505,4506,5000,5001,5004,5005,5037,5432,5500,5601,5666,5667,5672,5800,5900,5984,5999,6000,6082,6379,6653,6660,6661,6662,6663,6664,6665,6666,6667,6668,6669,6888,6888,7474,7777,8000,8001,8002,8005,8008,8080,8089,8081,8123,8139,8140,8172,8222,8333,8443,8889,8983,8999,9000,9001,9006,9042,9050,9051,9092,9200,9500,9800,9999,10050,10051,11211,11214,11215,15672,18091,18092,27017,27018,27019,28015,29015,33848,35357}

       var Usage = func() {
         fmt.Fprintf(CommandLine.Output(), "Usage of %s:\n", os.Args[0])
          flag.PrintDefaults()
        }

        var mode = flag.Int("m", 1, "Scanning mode. 1: Quick list. 2: NMAP TCP top 100 list. 3: Full portlist. Default: 1")
        var host = flag.String("u", "", "Host URL (including ending /)")
		    var verbose = flag.Bool("v", false, "Verbose mode ")
        flag.Parse()

        if *host == "" {
          Usage()
          os.Exit(1)
        }

          url := (*host + "AdminTools/querybuilder/logon?framework=")


        if *mode == 1 {
           for i:= 0;i < len(intSlice); i++ {
        makeRequest(strconv.Itoa(intSlice[i]), url, *verbose)
         }
        } else if *mode == 2 {
          for i:=0;i < len(mediumList);i++ {
            makeRequest(strconv.Itoa(mediumList[i]), url, *verbose)
            }
        } else if *mode == 3 {
          for i:=0;i < portLength;i++ {
            makeRequest(strconv.Itoa(i), url, *verbose)
            }
        }
        
       
       

    
         
 }