#!/usr/bin/env python
# -*- coding: utf-8 -*
#################################
import sys, os, platform, codecs
import socket
from multiprocessing.dummy import Pool					     	
from time import time as timer	
import time
import subprocess
import random
from ftplib import FTP

#################################
##########################################################################################
ktnred = '\033[31m'
ktngreen = '\033[32m'
ktn3yell = '\033[33m'
ktn4blue = '\033[34m'
ktn5purp = '\033[35m'
ktn6blueblue = '\033[36m'
ktn7grey = '\033[37m'
CEND = '\033[0m'        
#####################################
##########################################################################################
try:
    with codecs.open(sys.argv[1], mode='r', encoding='ascii', errors='ignore') as f:
        ooo = f.read().splitlines()
except IndexError:
    print (ktnred + '[+]================> ' + 'USAGE: '+sys.argv[0]+' listsite.txt' + CEND)
    pass
ooo = list((ooo))
##########################################################################################
def urlfix(url):
	if url[-1] == "/":
		pattern = re.compile('(.*)/')
		site = re.findall(pattern,url)
		url = site[0]
	if url[:7] != "http://" and url[:8] != "https://":
		url = "http://" + url
	return url

##########################################################################################
def connect(ip):
	try:
		ftp = FTP(ip, timeout=20)
		ktn1 = ftp.login()
		#ss = ftp.sendcmd('site cpfr')
		if 'Anonymous access granted' in ktn1:
			try:
				ss = ftp.sendcmd('site cpfr')
				print(ktn5purp + 'VULN ProFTPd on IP: '+ ip + CEND)
				open('VULNFTP.txt', 'a').write(ip+'\n')
				pass
			except:
				print(ktnred + "500 'SITE CPFR' not understood ---> IP: " + ip + ' NOT VULN' + CEND)
				pass
			pass
		else:
			print(ktnred + 'NO ProFTPD in IP: ' + ip + CEND)
	except:
		print(ktnred + 'ER0R2' + CEND)
		pass
	pass
##########################################################################################
def ftpscanner(ip):
	try:
		print(ktn7grey + 'Tryin to scan FTP for IP: ' + ip + CEND)
		scan = "nmap -p 21 -sV %s" % ip
		doscan = subprocess.check_output(scan, shell=True);
		if 'ProFTPD' in doscan:
			print(ktnred + 'ProFTPD FOUNDED ---> CHECKING THE VULN ...' + CEND)
			open('ftpD.txt', 'a').write(ip+'\n')
			connect(ip)
			pass
		else:
			print(ktnred + 'NO ProFTPD in IP: ' + ip + CEND)
		pass
	except:
		print(ktnred + 'ER0R1' + CEND)
		pass
	pass
##########################################################################################
def checkvalidip(ip):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.settimeout(2)
	killz = sock.connect_ex((ip,21))
	if killz == 0:
		print (ktngreen + '[+]=[ GOOD IP: '+ ip +' ]=[+]' + CEND) 
		ftpscanner(ip)
	else:
		print (ktn5purp +'[+]=[SOORY NOT FTP: ' + ip +' ]=[+]' + CEND)
	pass
##########################################################################################
def ktn(url):
	url = urlfix(url)
	print (ktnred + '[*]=[ ' + url + ' ]=[*]' + CEND)
	spltAr = url.split("://");
	i = (0,1)[len(spltAr)>1];
	dm = spltAr[i].split("?")[0].split('/')[0].split(':')[0].lower();
	ip = socket.gethostbyname(dm)
	checkvalidip(ip)
##########################################################################################
def logo():
	clear = "\x1b[0m"
	colors = [36, 32, 34, 35, 31, 37]
	x = ''' 
	     FEDERATION BLACK HAT SYSTEM | 0x666
                              ...
           s,                .                    .s
            ss,              . ..               .ss
            'SsSs,           ..  .           .sSsS'
             sSs'sSs,        .   .        .sSs'sSs
              sSs  'sSs,      ...      .sSs'  sSs
               sS,    'sSs,         .sSs'    .Ss
               'Ss       'sSs,   .sSs'       sS'
      ...       sSs         ' .sSs'         sSs       ...
     .           sSs       .sSs' ..,       sSs       .
     . ..         sS,   .sSs'  .  'sSs,   .Ss        . ..
     ..  .        'Ss .Ss'     .     'sSs. ''        ..  .
     .   .         sSs '       .        'sSs,        .   .
      ...      .sS.'sSs        .        .. 'sSs,      ...
            .sSs'    sS,     .....     .Ss    'sSs,
         .sSs'       'Ss       .       sS'       'sSs,
      .sSs'           sSs      .      sSs           'sSs,
   .sSs'____________________________ sSs ______________'sSs,
.sSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS'.Ss SSSSSSSSSSSSSSSSSSSSSs,
                        ...         sS'
                         sSs       sSs
                          sSs     sSs       - KTN
                           sS,   .Ss
                           'Ss   sS'
                            sSs sSs
                             sSsSs
                              sSs
                               s   
                                      KILL THE NET
                                     FB: fb/KtN.1990  
               Note! : PRIVATE CVE-2019-12815 SCANNER '''

	for N, line in enumerate(x.split("\n")):
		sys.stdout.write("\x1b[1;%dm%s%s\n" % (random.choice(colors), line, clear))
		time.sleep(0.05)
		pass


logo()
##########################################################################################
def Main():
	try:
		
		start = timer()
		ThreadPool = Pool(50)
		Threads = ThreadPool.map(ktn, ooo)
		print('TIME TAKE: ' + str(timer() - start) + ' S')
	except:
		pass

if __name__ == '__main__':
	Main()
