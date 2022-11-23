# CVE-2021-46069

### Exploit Title: Vehicle Service Management System - 'Mechanic List' Stored Cross Site Scripting (XSS)
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46069
### CVSS: 4.8 MEDIUM
### References: 
- https://www.plsanu.com/vehicle-service-management-system-mechanic-list-stored-cross-site-scripting-xss
- https://nvd.nist.gov/vuln/detail/CVE-2021-46069
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46069

### Description:
A Stored Cross Site Scripting (XSS) vulnerability exists in Vehicle Service Management System 1.0 via the Mechanic List Section in login panel.

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Mechanic List section and click on Create New button. 
3. Inject the below payload in Full Name & Contact input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

4. Click on Save button.
5. Malicious javascript code triggered.

### Impact:
An attacker can able to inject malicious JavaScript code in Mechanic List Section.

### Mitigation:
It is recommended to sanitize all the input fields throughout the application.
