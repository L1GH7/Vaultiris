# Struts2Scanner

Struts2Scanner is a vulnerability scanner to find out if a target endpoint is vulnerable to Remote Code Execution. Currently it checks against following vulnerabilities.

* cve-2020-17530
* cve-2019-0230
* cve-2018-11776 
* cve-2017-5638
* cve-2017-9791
	


## How to use
```
root@kali:/home/Struts2Scanner# python3 Struts2Scanner.py -h
usage: Struts2Scanner.py [options] --url "http://www.site.com/vuln.php?id=1"

optional arguments:
  -h, --help         show this help message and exit
  -u URL, --url URL  Target URL (e.g."http://www.site.com/vuln.php?id=1&fname=test&lname=tester")
  --data DATA        Data string to be sent through POST (e.g. "id=1&fname=test&lname=tester")
  --cookies COOKIES  HTTP cookies (eg. "jsessionid=1234")
  --proxy PROXY      Use a proxy to connect to the target URL

```
![Capture](/Capture.PNG)
## Requirements
* Python3
* Check requirements.txt file

## Installation Steps
```pip3 install -r requirements.txt```


## Legal Disclaimer
This project is made for educational and ethical testing purposes only. Usage of Struts2Scanner for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program.


### License
The project is licensed under MIT License.
