# CVE-2020-13159 - Artica Proxy before 4.30.000000 Community Edition allows Multiple OS command injection.

**Product Description:** Artica Tech offers a powerful but simple-to-use solution, usually the preserve of Large and Multinational companies. With a starting price of just 99€ and more than 62 000 active servers, Artica Proxy has been developed over the past 10 years as an Open Source Project to help SMEs and public bodies protect both their organizations and employees from Internet danger at a low cost.

**Description:** Artica Proxy before 4.30.000000 Community Edition allows OS command injection via the *Netbios name, Server domain name, dhclient_mac, Hostname, or Alias * field. 

**Vulnerability Type Other:** OS Command Injection

**Severity Rating:** High

**Vendor of Product:** Artica

**Affected Product Code Base:** Artica-Proxy - v4.28.030418 Community Edition

**Affected Component:** In Artica Proxy Web interface URIs, application ask user to enter with the value, a malicious user can perform OS command injection attack via below vulnerable parameters. *Netbios name, Server domain name, dhclient_mac, Hostname & Alias*

**Attack Type:** Remote

**Impact Information Disclosure:** True

**CVE Impact Other:** An attacker can add additional commands or inject operating system commands. This means that during a command injection attack, an attacker can easily take complete control of the host operating system of the web.

**Attack Vectors:** To exploit this type of vulnerability on the Artica-Proxy product, an attacker has to inject OS command at vulnerable parameters of an application.

**Has vendor confirmed or acknowledged the vulnerability?:**  True

**Reference:** https://sourceforge.net/projects/artica-squid/files/

**Exploit Author: Amar Kaldate**

**Contact:** https://www.linkedin.com/in/amar-kaldate-6aa744a9/


