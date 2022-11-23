# CVE-2020-15053 : Artica Proxy before 4.28.030418 Community Edition allows Reflected Cross Site Scripting.

**Product Description:** Artica Tech offers a powerful but simple-to-use solution, usually the preserve of Large and Multinational companies. With a starting price of just 99€ and more than 62 000 active servers, Artica Proxy has been developed over the past 10 years as an Open Source Project to help SMEs and public bodies protect both their organizations and employees from Internet danger at a low cost.

**Description:** Artica Proxy before 4.28.030418 Community Edition allows Reflected XSS via search field, Real time request, System Events, Proxy Objects and Firewall objects.

**Vulnerability Type:** Reflected Cross Site Scripting

**Vulnerability Description:** Reflected XSS attacks, also known as non-persistent attacks, occur when a malicious script is reflected off of a web application to the victim's browser. The script is activated through a link, which sends a request to a website with a vulnerability that enables execution of malicious scripts.

**Severity Rating:** High

**Vendor of Product:** Artica

**Affected Product Code Base:** Artica-Proxy - Before v4.28.030418 Community Edition

**Affected Component:** Several input fields are vulnerable to Reflected Cross Site Scripting via Search Field - Real time request,System Events, Proxy Events, Proxy Objects,Firewall Objects.

**Attack Type:** Remote

**Impact Information Disclosure:** True

**Attack Vector:** <input> tag, we can execute the attack by entering the malicious sql to view unauthorized viewing database data.
			   Used payload: _test"><img src=x onerror=alert('XSS')>_
			   
**Has vendor confirmed or acknowledge the vulnerability:** True

**Reference:** https://sourceforge.net/projects/artica-squid/files/

**Exploit Author:** Pratiksha Dhone

**Contact:** linkedin.com/in/pratiksha-dhone-56261b100
