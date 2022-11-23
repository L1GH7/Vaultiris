# CVE-2022-24449
Solar Appscreener XXE
[Suggested description]
An issue was found in Solar AppScreener SAST tool through 3.10.4. An unauthorized actor, may exploit effected hosts where vulnerable version is installed, by uploading specially crafted XML files on hosts, which has an expired or non-installed license. The lowest approved impact is XXE-SSRF.
------------------------------------------
[Additional Information]
Fixed in >3.10.4
------------------------------------------
[VulnerabilityType Other]
CWE-611: Improper Restriction of XML External Entity Reference
------------------------------------------
[Vendor of Product]
SOLAR SECURITY LLC (https://en.rt-solar.ru/)
------------------------------------------
[Affected Product Code Base]
Affected version: Solar Appscreener 3.10.4
------------------------------------------
[Affected Component]
License update mechanism
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
An attacker able to upload specially crafted XML file via license update function
------------------------------------------
[Discoverer]
Dmitry Kuramin (Jet Infosystems, jet.su)
------------------------------------------
[Reference]
https://jet.su
