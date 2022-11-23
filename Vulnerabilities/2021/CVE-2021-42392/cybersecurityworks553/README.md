# CVE-2021-42392-Detect
# About
The script detects vulnerable H2 server for the give list of IPs, it can identify the H2 Console web pages and check for access restrictions.


### Usage

Create a file containing list of IPs and name the file IP.txt, place the 'IP.txt' and 'h2-detect.py' in a directory and execute the script.

Here "IP.txt" is the default filename that's hardcoded into script however the script can also take an random filename as input argument and
when if no filename is specified it searched for "IP.txt" file in the current directory and if it is not found it throws an file not found error.

So either specify a filename or simply place IP.txt file with the h2_detect.py scripts. 

![usage1](https://github.com/cybersecurityworks553/CVE-2021-42392-Detect/blob/main/Detect_usage1.png)

![usage2](https://github.com/cybersecurityworks553/CVE-2021-42392-Detect/blob/main/Detect_usage2.png)

### Script Syntax
```
# Useing Default filename  
`$ python h2_detect_v0.3.py IP.txt ` or  `$ python h2_detect_v0.3.py`

# Specifying filename
`$ python3 h2_detect_v0.3.py <filename.txt>`
```
#### Error
![error](https://github.com/cybersecurityworks553/CVE-2021-42392-Detect/blob/main/Detect_error1.png)

### Requirments
pip install bs4


### Created by
Vivek Gopal
#### Security Analyst
#### Cybersecurityworks
