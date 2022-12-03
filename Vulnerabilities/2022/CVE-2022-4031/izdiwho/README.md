# CVE-2022-40317

Stored XSS in Notes

A stored XSS was found in the notes section of the application while we were pentesting a client that was using OpenKM. Basically, when storing notes, somehow normal HTML was allowed and it was rendered as is. As you know, <script> tags and javascript on-* events were being sanitized/replaced as part of the XSS sanitization ruleset.

However, we found that using "javascript&colon:" in an <a> tag bypassed that sanitization/replace rule.

To reproduce the issue, you can submit the following payload in the notes section.

`<a href='javascript&colon;alert(1)'>Click here for more info</a>`

To trigger the payload, a user will need to get tricked into clicking it.

Impact-wise: A low level user can submit malicious javascript payloads to log keystrokes, or redirect users to malicious websites, or submit data in the context of the victim, that gets triggered when the victim clicks on the link.

Affected areas in the application:
All notes sections.

Affected versions:
OpenKM Community Edition 6.3.11

CVSS 3.1:
CVSS:3.0/AV:N/AC:L/PR:L/UI:R/S:C/C:L/I:L/A:N (Medium 5.4)

Recommendation:
Improve sanitization, perhaps, by entirely replacing the 'javascript' keyword. Or just add an extra rule to replace 'javascript&colon;'
  
References:
Fix: https://github.com/openkm/document-management-system/pull/336
CVE:
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-40317
NIST:
https://nvd.nist.gov/vuln/detail/CVE-2022-40317
