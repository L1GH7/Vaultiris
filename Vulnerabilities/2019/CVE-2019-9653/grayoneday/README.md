# CVE-2019-9653

## Date
* Disclosure: 03/11/2019
* Last updated: 03/15/2019

## Summary
NUUO Inc. is a company providing a video-centric surveillance solution. They have many NVR (Network Video Recorder) products for different customers with various requirements. These NVRs are Linux embedded video recording systems that can manage several cameras. Nowadays, they are used worldwide by many public institutions, companies, banks, or individuals, etc. 
The web interface of these NVR systems contains a lot of critical vulnerabilities can be abused by unauthenticated attackers.  We discover that some vulnerable PHP scripts are lack of authentication mechanism and input protection thus they could be abused to achieve remote code execution on NUUO's devices as root.

## Range of Affected Product
Firmware version from 1.7.x to 3.3.x

## Suggestion
Update to a newer version. The latest firmware version is 3.10.x.

## Technical Details
The target that we were tested is firmware version 2.3.x. The following figure show the system information.
![image](https://github.com/grayoneday/CVE-2019-9653/blob/master/pic/systeminfo.png)

For this vulnerability, you can modify the request parameters then forward to the target and turn Burpsuite interceptor from on to off. You can find that the target machine has responded command result in Burpsuite history message, refer to the following figure.
![image](https://github.com/grayoneday/CVE-2019-9653/blob/master/pic/PoC1.PNG)

Checking the permission of running command.
![image](https://github.com/grayoneday/CVE-2019-9653/blob/master/pic/PoC2.PNG)

## Discoverer
* GrayOD (grayoneday@gmail.com)
* Endpoint Security Team @ NCCST (https://www.nccst.nat.gov.tw/)

## Acknowledgement
* Alan Chung (https://twitter.com/Se7en_5_Sec)
* Endpoint Security Team @ NCCST (https://www.nccst.nat.gov.tw/)
---
