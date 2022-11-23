# CVE-2021-46005
## **All Details about CVE-2021-46005**

Software: Online Car Rental System 1.0

Software Link: https://www.sourcecodester.com/cc/14145/online-car-rental-system-using-phpmysql.html

Vulnerability Type: Stored Cross Site Scripting

Affected Component: vehicalorcview in post-avehical page

Impact Denial of Service: True

Impact Code execution : True

Attack Type: Remote

Vendor of Product: Sourcecodester

## Description:
Cross-site scripting vulnerabilities occur when a parameter under the user’s control is either reflected to the user, stored and returned at a later time, or executed as a result of modifying the DOM environment. The vulnerability exists in Sourcecodester Online Car Rental System 1.0 in vehicalorcview parameter found during Adding new Vehical in Post Vehical page. Simply adding the simple payload <script>alert("CAR")</script> in vehicalcrview parameter, the application store the payload without input validatoin in database and whenever the client visit the page payload executed

The Affected URL where the  vulnerable parameter can be found : http://HOST/car-rental/admin/post-avehical.php

Impact: This vulnerability allows an attacker  to Hijacked session, Steal Credentials, access to client computers installing Malware in client's computer 

## More Info:
https://www.exploit-db.com/exploits/49546
