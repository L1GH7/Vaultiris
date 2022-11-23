## CVE-2020-25749

[Suggested description]
The Telnet service of Rubetek cameras RV-3406,
RV-3409, and RV-3411 cameras (firmware versions v342, v339) could allow an remote
attacker to take full control of the device with a high-privileged
account. The vulnerability exists because a system account has a
default and static password. The Telnet service cannot be disabled and this
password cannot be changed via standard functionality.
------------------------------------------
[Additional Information]
A letter was sent to the vendor about the vulnerability.
------------------------------------------
[VulnerabilityType Other]
CWE-798: Use of Hard-coded Credentials
------------------------------------------
[Vendor of Product]
Rubetek (https://rubetek.com/)
------------------------------------------
[Affected Product Code Base]
Camera RV-3406 - Firmware version 339 and 342 are affected. There are no fixed versions
Camera RV-3409 - Firmware version 339 and 342 are affected. There are no fixed versions
Camera RV-3411 - Firmware version 339 and 342 are affected. There are no fixed versions
------------------------------------------
[Affected Component]
Telnet service
------------------------------------------
[Attack Type]
Remote
------------------------------------------
[Impact Code execution]
true
------------------------------------------
[Impact Denial of Service]
true
------------------------------------------
[Impact Escalation of Privileges]
true
------------------------------------------
[Impact Information Disclosure]
true
------------------------------------------
[Attack Vectors]
Anyone with network access to cameras can connect to the Telnet service using a telnet client using the default password and get shell with root privileges.
------------------------------------------
[Discoverer]
Sergey Zelensky (Jet Infosystems, jet.su)
------------------------------------------
[Reference]
https://jet.su
