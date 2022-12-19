# cve-2020-15956
ACTi NVR 2.3 Standard/Professional Server and ACTi NVR3 Standard/Professional Server allows remote unauthenticated attackers to conduct a denial of service. To exploit this vulnerability merely send a malformed authorization header payload of at least 760-bytes to the Media Server triggering a buffer overflow and application termination of ActiveMediaServer.exe.

Vulnerable versions (possibly others):
* ACTi NVR3 Standard Server V.3.0.12.42
* ACTi NVR Professional V.2.3.04.07

Expected outcome: Denial of service. Server loss.

Fixed by vendor in NVR3 V.3.0.15.50

## Running the Exploit
```
python3 cve-2020-15956.py http://address
```

![PoC GIF](poc.gif)


## Resources
https://www.acti.com/DownloadCenter
