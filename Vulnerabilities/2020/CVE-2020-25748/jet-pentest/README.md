## CVE-2020-25748

[Suggested description]
A Cleartext Transmission issue was discovered
on Rubetek RV-3406, RV-3409, and
RV-3411 cameras (firmware versions v342, v339).
Someone in the middle can intercept and modify the video data from the
camera, which is transmitted in an unencrypted form. One can also modify
responses from NTP and RTSP servers and force the camera to use the
changed values.
------------------------------------------
[Additional Information]
A letter was sent to the vendor about the vulnerability.
------------------------------------------
[VulnerabilityType Other]
CWE-319: Cleartext Transmission of Sensitive Information
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
RTP service, NTP client, DNS client
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
[Attack Vectors]
Someone in the middle can intercept and modify the video data from the camera. You can also modify responses from NTP and RTSP servers and force the camera to use the changed values.
------------------------------------------
[Discoverer]
Sergey Zelensky (Jet Infosystems, jet.su)
------------------------------------------
[Reference]
https://jet.su
