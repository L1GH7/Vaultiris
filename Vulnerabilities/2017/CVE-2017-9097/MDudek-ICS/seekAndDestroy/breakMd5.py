# -*- coding: utf-8 -*-
import hashlib
from broken import *

class Colors:
    BLUE 		= '\033[94m'
    GREEN 		= '\033[32m'
    RED 		= '\033[0;31m'
    DEFAULT		= '\033[0m'
    ORANGE 		= '\033[33m'
    WHITE 		= '\033[97m'
    BOLD 		= '\033[1m'
    BR_COLOUR 	= '\033[1;37;40m'


# hashlib.md5("fred".encode("utf")).hexdigest()

def makeMd5(text):
	theHash = hashlib.md5(text.encode("utf")).hexdigest()
	return theHash

def bruteDic(hsh, planeUsr):
	for xPass in bruteList_1:
		testCred = (xPass + planeUsr)
		toMd5 = makeMd5(testCred)

		if toMd5 == hsh:
			yess = Colors.ORANGE+(xPass)+Colors.DEFAULT
			return yess

	return "fuck"

def getMD5(theMd5, user):


	user = user.replace("[","").replace("]","")
	lenUser = len(user)

	try:
		rawCred = brokenList[theMd5]

		brokenPass = rawCred[:-lenUser]

		broken = Colors.BLUE+"[ok]: "+Colors.GREEN+brokenPass+Colors.BLUE+"\t: "+Colors.GREEN+user+Colors.DEFAULT
		return broken

		exit(0)

	except:
		# user == passwd ------------------------------ # 
		pass1 = user
		testCred1 	= (pass1 + user)
		
		pass2 = ((user).upper())
		testCred2 	= (pass2 + user)
		
		pass3 = ((user).lower())
		testCred3 	= (pass3 + user)


		
		posiblePass1 = makeMd5(testCred1)
		posiblePass2 = makeMd5(testCred2)
		posiblePass3 = makeMd5(testCred3)
		
		# --------------------------------------------- # 

	if ( posiblePass1 == theMd5):

		broken = Colors.BLUE+"[ok]: "+Colors.GREEN+pass1+Colors.BLUE+"\t: "+Colors.GREEN+user+Colors.DEFAULT
		return broken

	elif ( posiblePass2 == theMd5):

		broken = Colors.BLUE+"[ok]: "+Colors.GREEN+pass2+Colors.BLUE+"\t: "+Colors.GREEN+user+Colors.DEFAULT
		return broken

	elif ( posiblePass3 == theMd5):

		broken = Colors.BLUE+"[ok]: "+Colors.GREEN+pass3+Colors.BLUE+"\t: "+Colors.GREEN+user+Colors.DEFAULT
		return broken


	else: 
		statBrute = bruteDic(theMd5, user)
		if statBrute != "fuck":
			broken = Colors.BLUE+"[ok]: "+Colors.GREEN+statBrute+Colors.BLUE+"\t: "+Colors.GREEN+user+Colors.DEFAULT
			return broken


		return Colors.RED+"not found"+Colors.DEFAULT
