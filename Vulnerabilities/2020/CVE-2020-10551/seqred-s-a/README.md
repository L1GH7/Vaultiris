**CVEID**: CVE-2020-10551

**Name of the affected product(s) and version(s)**: QQBrowser (all versions prior to 10.5.3870.400)

**Problem type**: CWE-284: Improper Access Control

---

**Summary**

QQBrowser is a web browser developed by Tencent. It is one of the most popular web browsers used in China.
During our tests, we have found a vulnerability which allows an unprivileged local attacker to gain code
execution as NT AUTHORITY\SYSTEM.
     
All version of QQBrowser prior to 10.5.3870.400 do not correctly set up ACLs for a TsService.exe file.
A malicious local attacker could overwrite the file to gain access to NT AUTHORITY\SYSTEM account, which
is the highest privileged account on a Windows system.
 
**Description**
 
QQBrowser creates a Windows service with ImagePath pointing to a TsService.exe file in its installation directory
(default: C:\Program Files (x86)\Tencent\QQBrowser\TsService.exe). This file’s permissions allow for writing by members
of NT AUTHORITY\Authenticated Users group which by default includes all users. An attacker could exploit the vulnerability
by replacing TsService.exe with his own executable, which would then be invoked with NT AUTHORITY\SYSTEM privileges.
 
**Reproduction**
 
Delete TsService.exe and replace it with a different program. Reboot the system.

**Remedy**

Install a newer version of QQBrowser.
