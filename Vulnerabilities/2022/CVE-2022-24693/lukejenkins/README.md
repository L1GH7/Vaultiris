# CVE-2022-24693
## Description

Baicells Nova436Q and Neutrino 430 devices with firmware through QRTB 2.7.8 have hardcoded credentials that are easily discovered, and can be used by remote attackers to authenticate via ssh. (The credentials are stored in the firmware, encrypted by the crypt function.)

## CVSS Score
~~This will be determined by the CNA that I am working with, but here is my "I'm not a security expert" interpretation of the variables on the NVD's calculator. I think that this scores somewhere between [9.4](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:L/A:H&version=3.1) and a [10.0](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:N/AC:L/PR:N/UI:N/S:C/C:H/I:H/A:H&version=3.1). Based on the security posture of the firmware, I think that this is a 10.0, but I'm not sure if that is considered for Scope or Integrety.~~

It ended up being rated as a [CVSS 9.8](https://nvd.nist.gov/vuln/detail/CVE-2022-24693)

## Vulnerability Type
[CWE-284: Improper Access Control](https://cwe.mitre.org/data/definitions/284.html) 

## Vendor of Product
BaiCells

## Affected Product Code Base
NOVA436Q - QRTB 2.7.8 (Likely all builds before 2.9.x) 
NEUTRINO430 - QRTB 2.7.8 (Likely all builds before 2.9.x) 

## Affected Component
Usernames and Passwords are static in the firmware, encrypted using crypt(), and so are crackable.

### Attack Type
Remote

### Impact Code execution
True

### Impact Denial of Service
True

### Impact Escalation of Privileges
True

### Impact Information Disclosure
True

### Attack Vectors
Out of the box, the firmware has static usernames and passwords.


## Reference
https://na.baicells.com/Service/Firmware
https://img.baicells.com//Upload/20211230/FILE/BaiBS_QRTB_2.7.8.IMG.IMG
https://img.baicells.com//Upload/20210909/FILE/98d2752f-6e83-49b1-9dab-d291e9023db6.pdf


## Has vendor confirmed or acknowledged the vulnerability?
Yes, though not publicly.


## Discoverer
Luke Jenkins


## Timeline
(timeline goes here)


## Details:

User "admin" password of [temporaraly "removed" at the request of the vendor] stored using unix crypt() DES algorithm.

Undocumented & non-configurable user "anonymous" password of [temporaraly "removed" at the request of the vendor] stored using unix crypt() DES algorithm.

Undocumented, non-configurable (but seemingly unusable for ssh) user "root" password of [temporaraly "removed" at the request of the vendor] stored using unix crypt() DES algorithm. Vendor claims this is unique per device, but this hasn't been confirmed.


## How to reproduce

1. Download firmware from the vendor's website, no login needed. As a customer, I love this; but from a security point of view it is important to understand how easy it is for a bad actor to get the firmware.
2. Run [binwalk](https://github.com/ReFirmLabs/binwalk) on the firmware image, a tool that extracts all of the files contained in the firmware. Snag the usernames and password hashes from /etc/passwd and /etc/shadow.
3. Use [hashcat](https://hashcat.net/hashcat/) to crack the hashes. In addition to being designed with 30+ year old CPUs in mind, the unix crypt() function "protecting" these passwords also limits them to 8 characters. Passwords go pop.
4. Credentials for the anonymous user can be used to ssh into the device. I'll save you the trouble of a port scan or looking through the config files from step 2, dropbear is listening on port 27149.

## Notes
This is my first published CVE, so please excuse (or pull request) any errors. 

Thanks to the folks at UETN for the assistance and backing on this one. 

Also thank you to the staff of Baicells NA for working with me on correcting this issue.
