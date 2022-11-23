## CVE-2020-25747

[Suggested description]
The Telnet service of Rubetek RV-3406, RV-3409, and
RV-3411 cameras (firmware versions v342, v339) can allow a remote
attacker to gain access to RTSP and ONFIV services without
authentication. Thus, the attacker can watch live streams from the
camera, rotate the camera, change some settings (brightness, clarity,
time), restart the camera, or reset it to factory
settings.
------------------------------------------
[Additional Information]
A letter was sent to the vendor about the vulnerability.
------------------------------------------
[Vulnerability Type]
Incorrect Access Control
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
ONVIF-service, RTSP-service
------------------------------------------
[Attack Type]
Remote
------------------------------------------
[Impact Denial of Service]
true
------------------------------------------
[Impact Information Disclosure]
true
------------------------------------------
[CVE Impact Other]
Onvif service possible to move the camera and change some settings (brightness, clarity, time), it can restart the device and activate the reset to factory settings
------------------------------------------
[Attack Vectors]
Anyone with network access to the camera can connect to ONVIF and RTSP services without using authentication.
------------------------------------------
[Discoverer]
Sergey Zelensky (Jet Infosystems, jet.su)
------------------------------------------
[Reference]
https://jet.su
