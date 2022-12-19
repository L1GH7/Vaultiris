# CVE-2020-15051 : Artica Proxy before 4.28.030418 Community Edition allows Stored Cross Site Scripting.

**Product Description:**
Artica Tech offers a powerful but simple-to-use solution, usually the preserve of Large and Multinational companies. With a starting price of just 99€ and more than 62 000 active servers, Artica Proxy has been developed over the past 10 years as an Open Source Project to help SMEs and public bodies protect both their organizations and employees from Internet danger at a low cost.

**Description:** Artica Proxy before 4.28.030418 Community Edition allows Cross Site Scripting exists via the input fields Server Domain Name,Your Email Address,Group Name,MySQL Sever,Database,MySQL Username, Group Name and Task description fields.

**Vulnerability Type:** Cross Site Scripting (XSS)

**Vulnerability Description:** 
Cross-Site Scripting (XSS) attacks are a type of injection, in which malicious scripts are injected into otherwise benign and trusted websites. XSS attacks occur when an attacker uses a web application to send malicious code, generally in the form of a browser side script, to a different end user.

**Severity Rating:** High

**Vendor of Product:** Artica

**Affected Product Code Base:** Artica-Proxy - before v4.28.030418 Community Edition

**Affected Component:** Several input fields are vulnerable to Stored Cross Site scripting. These are few affted components field name, Server Domain Name,Your Email Address,Group Name,MySQL Sever,Database,MySQL Username, Group Name and Task description fields.We can gain the access of user session cookie and can able to perform malicious activity.

**Attack Type:** Remote

**Impact Information Disclosure:** True

**Attack Vector:** <input> tag, we can execute the attack by entering the malicious javascript code to gain the access of user session cookie.
			   Used payload: *_test"><svg/onload=alert(1)>_*
			   
**Has vendor confirmed or acknowledge the vulnerability:** True

**Reference:** https://sourceforge.net/projects/artica-squid/files/

**Exploit Author:** Pratiksha Dhone

**Contact:** linkedin.com/in/pratiksha-dhone-56261b100
