# CVE-2020-15052 : Artica Proxy before 4.28.030418 Community Edition allows SQL Injection.

**Product Description:** Artica Tech offers a powerful but simple-to-use solution, usually the preserve of Large and Multinational companies. With a starting price of just 99€ and more than 62 000 active servers, Artica Proxy has been developed over the past 10 years as an Open Source Project to help SMEs and public bodies protect both their organizations and employees from Internet danger at a low cost.

**Description:** Artica Proxy before 4.28.030418 Community Edition allows SQL Injection exists via the input fields Netmask,Hostname and Alias field.

**Vulnerability Type:** SQL Injection

**Vulnerability Description:** A SQL injection attack consists of insertion or “injection” of a SQL query via the input data from the client to the application. ... SQL injection attacks are a type of injection attack, in which SQL commands are injected into data-plane input in order to effect the execution of predefined SQL commands.

**Severity Rating:** High

**Vendor of Product:** Artica

**Affected Product Code Base:** Artica-Proxy - before v4.28.030418 Community Edition

**Affected Component:** Several input fields are vulnerable to SQL Injection attack, Netmask, Hostname, Alias field.  

**Attack Type:** Remote

**Impact Information Disclosure:** True

**Attack Vector:** <input> tag, we can execute the attack by entering the malicious sql to view unauthorized viewing database data.
			   Used payload: _1" GROUP BY 1,2,--_
			   
**Has vendor confirmed or acknowledge the vulnerability:** True

**Reference:** https://sourceforge.net/projects/artica-squid/files/

**Exploit Author:** Pratiksha Dhone

**Contact:** linkedin.com/in/pratiksha-dhone-56261b100

