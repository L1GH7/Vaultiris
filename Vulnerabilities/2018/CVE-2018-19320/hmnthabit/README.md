# CVE-2018-19320 LPE Exploit

## Description

Local Privilege Escalation Exploit of CVE-2018-19320. The exploit uses the exposed functions in **gdrv.sys** that allow a low-level user to allocate and write data to memory for escalating the privileges to SYSTEM. 



## ScreenShot

**Tested on:**  Windows 10 x64 21H1 (OS Build 1903.1165)

![image-20210819200340188](img/image-20210819200340188.png)



## Affected Versions

- GIGABYTE APP Center v1.05.21 and previous
- AORUS GRAPHICS ENGINE v1.33 and previous
- XTREME GAMING ENGINE v1.25 and previous
- OC GURU II v2.08



## Credits

The vulnerability was discoverd by Diego Juarez

Adviosry: https://www.secureauth.com/labs/advisories/gigabyte-drivers-elevation-of-privilege-vulnerabilities/

