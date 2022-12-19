# CVE-2020-25515
#Unrestricted File Upload in Simple Library Management System 1.0

#Vendor - https://www.sourcecodester.com

#Product -https://www.sourcecodester.com/php/14439/simple-library-management-system-project-using-phpmysql.html

#Vulnerability Type - Unrestricted File Upload

#Affected Component - Books > New Book ,[ http://<site>/lms/index.php?page=books] http://<site>/lms/index.php?page=books

#Attack Type- Local

#Impact Code execution - true

#Attack Vectors 

1) Login to Dashboard, go to Books tab and Add New Book.

2) in upload field, upload "php-reverse-shell" (https://github.com/pentestmonkey/php-reverse-shell/blob/master/php-reverse-shell.php) instead of books.

3) listen in Kali terminal with port 1234, and then try to edit this card.

4) listen in Kali terminal with port 1234

5) if you didn't get shell, right click on broken image and open this, we can see our uploaded file is successfully executed and got connect back shell
