# DNS Zone Transfer Test
Automates Domain Name System (DNS) zone transfer testing. 

Automatically finds a given domains nameservers, and 
tests for zone transfers, if successfully, identified A records are printed to standard output.

# Usage
```
dztt zonetransfer.me
dztt -f domains.txt
dztt -n 1.1.1.1 zonetransfer.me
```

# Requirements
```
git >= 2.36.0
python >= 3.9
pip >= 20.3.4
```

# Installation
Download the repository change to the relevant directory, and install the PIP package.
```
git clone https://github.com/Rodney-O-C-Melby/zone-transfer-tester.git  
cd dns-zone-transfer-test
pip install .
```  
Then add the python scripts directory to your PATH environment variable.
```
python get_scripts_path.py
```
Add the output to PATH or only use dztt within this directory using:
```
python src/dns-zone-transfer-test/dztt
```
# Help
```
usage: dztt [-h] [-f FILE] [-n NAMESERVER] target

DNS Zone Transfer Test: dztt tests IP's or domains for zone transfers. (XFR)

positional arguments:
  target                Target IP or Domain name to test

options:
  -h, --help            show this help message and exit
  -f FILE, --file FILE  file of domain names to test, one per line
  -n NAMESERVER, --nameserver NAMESERVER
                        DNS IP for target DNS query, defaults to local DNS

### EXAMPLES ###
dztt  
dztt zonetransfer.me
dztt -f domains.txt  
dztt -n 1.1.1.1 zonetransfer.me  
dztt -f domains.txt -n 1.1.1.1
```  
# Output
A domain that is vulnerable to zone transfers will print the following message to standard out, and print each 
identified subdomain.
``` 
zonetransfer.me is VULNERABLE to DNS Zone Transfers!!!  
Found 34 subdomains at zonetransfer.me  
``` 
A domain that is not vulnerable to zone transfers will print the following message to standard out.
``` 
nmap.org is SECURE against DNS Zone Transfers :-)  
```
or 
``` 
nmap.org is VERY SECURE against DNS Zone Transfers :-)  
```
# Input
Accepts a file containing domain names to test, one per line.
``` 
dztt -f domains.txt
```
# PIP Requirements
```
dnspython >= 2.2.1
```