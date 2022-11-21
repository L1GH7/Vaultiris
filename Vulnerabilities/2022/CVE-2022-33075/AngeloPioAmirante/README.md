# CVE-2022-33075



# Exploit Title: Zoo Management System 1.0 - Stored Cross-Site-Scripting (XSS)
# Date: 05/26/2022
# Exploit Author: Angelo Pio Amirante
# Vendor Homepage: https://www.sourcecodester.com/
# Software Link: https://www.sourcecodester.com/php/15344/zoo-management-system-phpoop-free-source-code.html
# Version: 1.0
# Tested on: Server: XAMPP
# CVE: 2022-33075

# Description:
Zoo Management System 1.0 is vulnerable to a stored cross site scripting in “Add Classification” functionality of the admin panel.

# Exploit:
1. Goto: http://localhost/admin/public_html/admin_login and login with the provided credentials
2. Goto: http://localhost/admin/public_html/save_classification
3. The “Classification Display Name” and “Classification Table Name” are both vulnerable so you can put <script>alert(“xss”)</script> in one of them
4. Goto: http://localhost/admin/public_html/view_classifications
5. Stored XSS payload is fired

# Image Poc:

- [XSS Payload](https://ibb.co/FXc5zLt)
- [XSS alert](https://ibb.co/CtFSQrQ)
