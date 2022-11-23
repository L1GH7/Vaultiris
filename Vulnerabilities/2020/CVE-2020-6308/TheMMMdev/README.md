# CVE-2020-6308
Exploit script for SAP Business Objects SSRF

This is a simple Golang script to automate the exploitation of CVE-2020-6308. The original Github repo did not show any automation (https://github.com/InitRoot/CVE-2020-6308-PoC , thanks @InitRoot), so this was made in an effort to help pentesters/red teamers to provide a proof of concept to clients. 

Warning: During the engagement this was created for, the specific webserver responded only after 10 seconds when a port was open. This specific parameter could change. If this is the case, only change the if condition in line 35.

Process could not be multithreaded due to the webserver only handling one request at a time and no accepting of a new request until first request is done. 
