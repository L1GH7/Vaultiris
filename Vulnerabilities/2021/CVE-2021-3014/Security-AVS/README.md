# CVE-2021-30146
Seafile 7.0.5 Persistent XSS

[Suggested description]: Application (Server Version: 7.0.5 Seafile) is vulnerable to Persistent XSS via share library functionality. 

[Additional Information]: Seafile is an open source, self-hosted file sync and share solution with high performance and reliability. 

[Vulnerability Type]: Cross Site Scripting (XSS)

[Vendor of Product]: https://www.seafile.com/en/home/

A letter was sent to the vendor about the vulnerability.

[Attack Type]: Remote

[Attack Vectors]: Attacker with local account has ability to share specially created library with malicious JavaScript code to other users. Malicious JavaScript code is executed via notification message in victim account. Attacker can attack all users in application via single try.

[Discovered]: Alexander Semenenko

[Reference]: https://www.seafile.com/

[Proof of Concept]:

![alt text](https://github.com/Security-AVS/CVE-2021-30146/blob/main/Persistent%20XSS.png)
