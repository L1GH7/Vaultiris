
#!/usr/bin/env python3
# NotProxyShell Scanner
# 2022 - ZephrFish 

import requests
import argparse
import re

# Colours
def prRed(skk): 
    return "\033[91m {}\033[00m" .format(skk)
def prGreen(skk): 
    return "\033[92m {}\033[00m" .format(skk)
def prCyan(skk): 
    return "\033[96m {}\033[00m" .format(skk)
def prYellow(skk): 
    return "\033[93m {}\033[00m" .format(skk)

def banner():
    print('''
     _   _       _   ____                      ____  _          _ _ 
    | \ | | ___ | |_|  _ \ _ __ _____  ___   _/ ___|| |__   ___| | |
    |  \| |/ _ \| __| |_) | '__/ _ \ \/ / | | \___ \| '_ \ / _ \ | |
    | |\  | (_) | |_|  __/| | | (_) >  <| |_| |___) | | | |  __/ | |
    |_| \_|\___/ \__|_|   |_|  \___/_/\_\\__,  |____/|_| |_|\___|_|_|
                                         |___/ 
    Scanner by ZephrFish                                                                         
    ''')
    print('''
    CVE-2022-40140 & CVE-2022-41082 Scanner
    Usage:
            Check Hosts: python3 NotProxyShell.py -u target_host -d domain.com -e email@domain.com
            Check List: python3 NotProxyShell.py -f file.txt -d domain.com -e email@domain.com
        ''')

# Initial variables
#outfile = None
targethost = 'example.com'
email = 'example@example.com'


banner()

def scanner(args,targethost,email,domain):
    targethost = args.targethost
    domain = args.targetdomain
    email = args.targetemail
    proxy = {

                "http": "http://127.0.0.1:8080",
                "https": "https://127.0.0.1:8080",
        }
    regex=re.compile('^http://|^https://')
    attack_url = targethost + f'/autodiscover/autodiscover.json?{email}/owa/&Email=autodiscover/autodiscover.json?b@{domain}&Protocol=TESTING&Protocol=PowerShell'
    if re.match(regex, attack_url):
        try:
            response = requests.post(url=attack_url, verify=False, timeout=5, proxy=proxy)
            if response.status_code == 200 and 'Powershell' in response.text:
                prGreen(f"[+] Target {targethost} Vulnerable")
            elif response.status_code != 200 and 'X-FEServer' in response.text:
                prYellow(f"[!] Target {targethost} might be Vulnerable")
            elif response.status_code == 503:
                prRed(f"[-] Target {targethost} is not Vulnerable, Mitigation Detected")
        
        except Exception as e:
            prYellow(f'url exception {targethost}')





def main(args):
    target = args.targethost
    if targethost == None:
        targethostfile = args.targethostfile
        with open(targethostfile.rstrip(), 'r') as f:
             for target in f:
                scanner(target)
    else: 
        target = args.targethost 
        scanner(target)




if __name__ == '__main__':
    
    parser = argparse.ArgumentParser()
    parser.add_argument('-u', '--targethost', default=None, required=False, help='Single Target host e.g zsec.uk')
    parser.add_argument('-f', '--targethostfile', default=None, required=False, help='File with targets, one per line')
    parser.add_argument('-e', '--email', default=None, required=False, help='Known email of org')
    parser.add_argument('-d', '--targetdomain', default=None, required=False, help='Known domain of the target org')
    
    args = parser.parse_known_args()
    
    main(args)