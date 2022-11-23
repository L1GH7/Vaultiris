# CVE-2019-15120
Exploit for XSS via BBCode on Kunena extension before 5.1.14 for Joomla!

# Information

Description: In Kunena extension before 5.1.14 for Joomla!, it’s possible for any user to execute a stored XSS attack by sending special message in any field with BBcode support. As a result, the vulnerability can easily leads to RCE.
Versions Affected: Kunena extension for Joomla! version 5.x through 5.1.14

Researcher: Andrey Skuratov (https://github.com/h3llraiser/)

Disclosure Link: https://www.kunena.org/blog/207-kunena-5-1-14-released

VEL Link: https://vel.joomla.org/resolved/2260-kunena-5-0-x-5-1-14-xss-cross-site-scripting

NIST CVE Link: https://nvd.nist.gov/vuln/detail/CVE-2019-15120

# POC

An example of vector:

```[spoiler="qwe<script>alert('PWNED!')</script>"]rty[/spoiler]```

# Screenshots:

![](POC_1_1.png)
![](POC_1_2.png)

# Stored XSS in Kunena extension before 5.1.14 for Joomla! to RCE video demonstration (BONUS)
[![rce](http://img.youtube.com/vi/xmEub6ZVUhI/0.jpg)](https://youtu.be/xmEub6ZVUhI)
