# PCSecure V5.0.8.xw - Use of Hard-coded Credentials in configuration files leads to admin panel access

## Summary
Vendor: PCTechSoft SAS

Product: PCSecure

Affected version(s): 5.0.8.xw

CodeName: PapiQuieroPollo00

## Vulnerability
Type: Use of Hard-coded Credentials (CWE-798)

CVSSv3.1 Vector: CVSS:3.1/AV:L/AC:L/PR:L/UI:N/S:U/C:H/I:H/A:H

CVSSv3.1 Base Score: 7.8 (High)

Exploit available: No

CVE ID: CVE-2022-42176

## Description
The configuration file (system.bmp) in PCSecure V5.0.8.xw stores the credentials for access to admin panel in plain text, which allows a local user without privileges to login in as administrator and execute commands as windows local admin or reduce security control via reading the file from Chrome or Firefox.

## Proof of Concept
### Demo
![demo](./w7-demo.gif)

### Steps to reproduce
1. Read the file system.bmp from Chrome using the following URI:
"view-source:file:///C:/Archivos%20de%20programa/ARCHIVOS%20COMUNES/CONFIG/system.bmp"

2. Identify the password for each user. The first line contains all the usernames separed by "*", the second line contains the password for each username.

3. Launch PCSecure admin panel.
Press Ctrl + Mayus + P or Execute "C:\archivos de programa\personal\pcsecure" or launch from windows Start menu.

4. Login as Admin

5. Change security profile and execute any command

### Tested on
Windows 7

## Exploit
There is no exploit for the vulnerability but can be manually exploited.

## Mitigation
There is currently no patch available for this vulnerability.

## Credits
The vulnerability was discovered by Oreocato aka [Miguel Chaparro Pedraza](https://www.linkedin.com/in/miguel-chaparro-pedraza)

## References
Vendor page: https://www.pctechsoft.net

## Timeline
2019-09-07: Vulnerability discovered.

2019-09-07: Vendor contacted.

2022-09-28: Public Disclosure.

2022-10-20: CVE ID assigned
