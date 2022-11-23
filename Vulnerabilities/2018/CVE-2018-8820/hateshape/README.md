![Supported Python versions](https://img.shields.io/badge/python-2.7-blue.svg)
# frevvomapexec
frevvomapexec is a script to verify the existence of a blind SQL injection vulnerability by injecting a delay of your choosing in seconds in Square 9 GlobalForms 6.2. To verify the vulnerability is legitimate, the end user will be required to do math (e.g. Subtracting the smaller number from the bigger number in seconds). If that math value aligns approximately with the parameter specified with (-s) you are the proud new owner of a SQL injection. Also, use SQLmap. It is your friend. Also, remember this is an authenticated SQL injection! But do not dispair oftentimes the server will still have default credentials enabled! By default freevomapexec uses the default credentials so oftentimes all is well!

## Usage ##
Usage: frevvomapexec.py [-h] -t TARGET -s SECONDS -o PORT [-u] [-p]

        Proof of Concept script for validation of CVE-2018-8820.
         - Type of issue: Authenticated Blind SQL injection
         - Product: Square 9 GlobalForms
         - Version: v6.2.x

##### Required: ##### 
    -t TARGET, --target TARGET        Target URL or IP Address
    -s SECONDS, --seconds SECONDS     Number of seconds to pause Frevvo
    -o PORT, --port PORT              Frevvo Web Server Port
  
##### Optional Arguments: #####
    -h, --help            show this help message and exit
    -u, --username        Login Username
    -p, --password        Login Password

