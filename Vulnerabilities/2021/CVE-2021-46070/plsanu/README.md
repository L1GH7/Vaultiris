# CVE-2021-46070

### Exploit Title: Vehicle Service Management System - 'Service Requests' Stored Cross Site Scripting (XSS)
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46070
### CVSS: 4.8 MEDIUM
### References: 
- https://www.plsanu.com/vehicle-service-management-system-service-requests-stored-cross-site-scripting-xss
- https://nvd.nist.gov/vuln/detail/CVE-2021-46070
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46070

### Description:
A Stored Cross Site Scripting (XSS) vulnerability exists in Vehicle Service Management System 1.0 via the Service Requests Section in login panel.

### Exploit:
1. Login to the admin panel http://localhost/vehicle_service/admin
2. Navigate to Service Requests section and click on Create New button. 
3. Inject the below payload in Owner Contact, Address, Vehicle Name, Vehicle Registration Number & Vehicle Model input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

4. Click on Save Request button.
5. Malicious javascript code triggered.
6. Navigate to Report section.
7. Malicious javascript code triggered.

### Impact:
An attacker can able to inject malicious JavaScript code in Service Requests Section.

### Mitigation:
It is recommended to sanitize all the input fields throughout the application.
