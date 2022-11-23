### [CVE-2022-0725](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-0725)
![](https://img.shields.io/static/v1?label=Product&message=keepass&color=blue)
![](https://img.shields.io/static/v1?label=Version&message=2.48&color=blue)
![](https://img.shields.io/static/v1?label=Vulnerability&message=InformationExposure&color=brighgreen)

# POC for KeePass [CVE-2022-0725]

Steps to Reproduce: </br>

Step 1: Run "journalctl -f" in a terminal window. </br>
Step 2: Double click a password in KeePass. </br>
Step 3: Wait for the clear timeout to trigger. </br>

Actual results: </br> 
See your plain text password logged in the terminal window </br> 

Expected results: </br>
Never see your plain text password logged anywhere

## Reference: 
https://bugzilla.redhat.com/show_bug.cgi?id=2052696

NOTE: Vulnerability Only Present in KeePass. Not in KeePassXC and Other Packages.
