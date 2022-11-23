# CVE-2021-46080

### Exploit Title: Vehicle Service Management System - 'Multiple' Cross-Site Request Forgery (CSRF) Leads to Stored Cross Site Scripting (XSS)
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46080
### CVSS: 4.8 MEDIUM
### References: 
- https://www.plsanu.com/vehicle-service-management-system-multiple-cross-site-request-forgery-csrf-leads-to-stored-cross-site-scripting-xss
- https://nvd.nist.gov/vuln/detail/CVE-2021-46080
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46080

### Description:
A Cross Site Request Forgery (CSRF) vulnerability exists in Vehicle Service Management System 1.0. An successful CSRF attacks leads to Stored Cross Site Scripting Vulnerability.

### 1. Vehicle Service Management System - 'Mechanic List' (/admin/?page=mechanics)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Create two admin accounts.
3. Login the Admin-1 account in Browser A (Chrome)
4. Login the Admin-2 account in Browser B (Firefox)
5. In Admin-1 account(Chrome) navigate to the Mechanic List section and click on Create New button.
6. Inject the below payload in Full Name input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

7. Click on Save button.
8. Capture the request in burpsuite and generate the CSRF Html File.
9. Save the CSRF Html file For Ex: CSRF.html
10. In Browser B (Firefox) browse the CSRF.html file.
11. Navigate to the Mechanic List section in Browser B (Firefox).
12. Malicious javascript code triggered.

### 2. Vehicle Service Management System - 'Service Requests' (/admin/?page=service_requests)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Create two admin accounts.
3. Login the Admin-1 account in Browser A (Chrome)
4. Login the Admin-2 account in Browser B (Firefox)
5. In Admin-1 account(Chrome) navigate to the Service Requests section and click on Create New button.
6. Inject the below payload in Owner Contact input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

7. Click on Save Request button.
8. Capture the request in burpsuite and generate the CSRF Html File.
9. Save the CSRF Html file For Ex: CSRF.html
10. In Browser B (Firefox) browse the CSRF.html file.
11. Navigate to the Service Requests section in Browser B (Firefox).
12. Choose the newly created Service Requests and click on Action under View.
13. Malicious javascript code triggered.

### 3. Vehicle Service Management System - 'Category List' (/admin/?page=maintenance/category)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Create two admin accounts.
3. Login the Admin-1 account in Browser A (Chrome)
4. Login the Admin-2 account in Browser B (Firefox)
5. In Admin-1 account(Chrome) navigate to the Category List section and click on Create New button.
6. Inject the below payload in Category Name input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

7. Click on Save button.
8. Capture the request in burpsuite and generate the CSRF Html File.
9. Save the CSRF Html file For Ex: CSRF.html
10. In Browser B (Firefox) browse the CSRF.html file.
11. Navigate to the Category List section in Browser B (Firefox).
12. Malicious javascript code triggered.

### 4. Vehicle Service Management System - 'Service List' (/admin/?page=maintenance/services)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Create two admin accounts.
3. Login the Admin-1 account in Browser A (Chrome)
4. Login the Admin-2 account in Browser B (Firefox)
5. In Admin-1 account(Chrome) navigate to the Service List section and click on Create New button.
6. Inject the below payload in Service Name input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

7. Click on Save button.
8. Capture the request in burpsuite and generate the CSRF Html File.
9. Save the CSRF Html file For Ex: CSRF.html
10. In Browser B (Firefox) browse the CSRF.html file.
11. Navigate to the Service List section in Browser B (Firefox).
12. Malicious javascript code triggered.

### 5. Vehicle Service Management System - 'User List' (/admin/?page=user/list)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Create two admin accounts.
3. Login the Admin-1 account in Browser A (Chrome)
4. Login the Admin-2 account in Browser B (Firefox)
5. In Admin-1 account(Chrome) navigate to the User List section and click on Create New button.
6. Inject the below payload in First Name input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

7. Click on Save button.
8. Capture the request in burpsuite and generate the CSRF Html File.
9. Save the CSRF Html file For Ex: CSRF.html
10. In Browser B (Firefox) browse the CSRF.html file.
11. Navigate to the User List section in Browser B (Firefox).
12. Malicious javascript code triggered.

### 6. Vehicle Service Management System - 'Settings' (/admin/?page=system_info)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Create two admin accounts.
3. Login the Admin-1 account in Browser A (Chrome)
4. Login the Admin-2 account in Browser B (Firefox)
5. In Admin-1 account(Chrome) navigate to the Settings section.
6. Inject the below payload in System Name input field.

### Payload:
```html
 "><script>alert(document.cookie)</script>
```

7. Click on Update button.
8. Capture the request in burpsuite and generate the CSRF Html File.
9. Save the CSRF Html file For Ex: CSRF.html
10. In Browser B (Firefox) browse the CSRF.html file.
11. Navigate to the Settings section in Browser B (Firefox).
12. Malicious javascript code triggered.

### Impact:
Cross-Site Request Forgery vulnerability exists in Multiple endpoints it leads to Stored Cross Site Scripting Vulnerability.

### Mitigation:
It is recommended to implement the following:
- Unpredictable with high entropy, as for session tokens in general.
- Tied to the user's session.
- Strictly validated in every case before the relevant action is executed.
