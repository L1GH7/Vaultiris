# CVE-2022-1051
WPQA &lt; 5.2 - Subscriber+ Stored Cross-Site Scripting via Profile fields

# Description
The plugin, used as a companion plugin for the Discy and Himer themes, does not sanitise and escape the city, phone or profile credentials fields when outputting it in the profile page, allowing any authenticated user to perform Cross-Site Scripting attacks.

# Proof of Concept
Edit your profile and add the following payload in one of the unescaped fields. ```<img src onerror=alert(/XSS/)>```
Upon visiting your profile, XSS will be triggered 


### Fixed in version 5.2

### References:

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-1051

https://wpscan.com/vulnerability/cb2fa587-da2f-460e-a402-225df7744765

### Video POC:

https://www.youtube.com/watch?v=hoy9MYoki7k
