# PACPoC: A local PoC exploit for CVE-2019-2205

To try out this exploit install the "malicious" app PacTest.apk and make `exploit.pac` available over http with, for example `python -m SimpleHTTPServer`. Then go to proxy settings on an Android device, select proxy autoconfig, and enter the URL to `exploit.pac`. 