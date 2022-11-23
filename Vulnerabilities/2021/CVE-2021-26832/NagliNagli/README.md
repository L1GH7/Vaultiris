# CVE-2021-26832

[Suggested description]
Cross Site Scripting (XSS) at the "Reset Password" page form of
Priority Enterprise Management System v8.00 allows attackers to execute
javascript on behalf of the victim by sending a malicious URL or
directing the victim to a malicious site.

------------------------------------------

[Vulnerability Type]
Cross Site Scripting (XSS)

------------------------------------------

[Vendor of Product]
https://www.priority-software.com/il/

------------------------------------------

[Affected Product Code Base]
Priority Enterprise Management System - Version 8.00

------------------------------------------

[Affected Component]
Reset password page form

------------------------------------------

[Attack Type]
Remote

------------------------------------------

[CVE Impact Other]
Javascript execution on the victims behalf

------------------------------------------

[Attack Vectors]
To exploit the vulnerability, the victim needs to click on a crafted link or webpage, which will initiate the CSRF request on his behalf.

------------------------------------------

[Reference]
https://galnagli.com / https://github.com/NagliNagli/CVE-2021-26832

------------------------------------------

[Discoverer]
Gal Nagli
