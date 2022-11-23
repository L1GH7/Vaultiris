# CVE-2022-26158

An issue was discovered in the web application in Cherwell Service
Management (CSM) 10.2.3. It accepts and reflects arbitrary domains
supplied via a client-controlled Host header. Injection of a malicious
URL in the Host: header of the HTTP Request results in a 302 redirect
to an attacker-controlled page.
