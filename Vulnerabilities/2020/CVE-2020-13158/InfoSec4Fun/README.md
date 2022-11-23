# CVE-2020-13158 - Artica Proxy before 4.30.000000 Community Edition allows Directory Traversal 

**Product Description:** Artica Tech offers a powerful but simple-to-use solution, usually the preserve of Large and Multinational companies. With a starting price of just 99€ and more than 62 000 active servers, Artica Proxy has been developed over the past 10 years as an Open 
Source Project to help SMEs and public bodies protect both their organizations and employees from Internet danger at a low cost.

**Description:**
Artica Proxy before 4.30.000000 Community Edition allows Directory Traversal via the fw.progrss.details.php popup parameter.

**Vulnerability Type:** Directory Traversal

**Severity Rating:** High

**Vendor of Product:** Artica

**Affected Product Code Base:** Artica-Proxy - v4.28.030418 Community Edition

**Affected Component:** For Directory Traversal attack *popup* parameter value is not sanitized/validate properly. In Artica-Proxy product some of the parameters are not sanitized/validate properly which allows an attacker to inject OS Commands.

**Attack Type:** Remote

**Impact Information Disclosure:** True

**Attack Vectors:**  To exploit this vulnerability attacker must enter pathname like
 ( https://<Web_Interface_URIs>/fw.progrss.details.php?popup=..%2f..%2f..%2f..%2f..%2f..%2fetc%2fpasswd )

**Has vendor confirmed or acknowledged the vulnerability?:** true

**Reference:** https://sourceforge.net/projects/artica-squid/files/

**Exploit Author: Amar Kaldate**

**Contact:** https://www.linkedin.com/in/amar-kaldate-6aa744a9/
