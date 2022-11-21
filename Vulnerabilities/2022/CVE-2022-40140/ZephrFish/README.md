# NotProxyShellScanner
Python implementation for NotProxyShell aka CVE-2022-40140 & CVE-2022-41082.

## Setup 
Install the requirements all that's required is python3 requests.

```
pip3 install -r requirements.txt
```

## Running
There are a few options when it comes to running the tooling:
```
usage: NotProxyShell.py [-h] [-u TARGETHOST] [-f TARGETHOSTFILE] [-e EMAIL] [-d TARGETDOMAIN]

optional arguments:
  -h, --help            show this help message and exit
  -u TARGETHOST, --targethost TARGETHOST
                        Single Target host e.g zsec.uk
  -f TARGETHOSTFILE, --targethostfile TARGETHOSTFILE
                        File with targets, one per line
  -e EMAIL, --email EMAIL
                        Known email of org
  -d TARGETDOMAIN, --targetdomain TARGETDOMAIN
                        Known domain of the target org
```