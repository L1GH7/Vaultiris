# CVE-2021-46075

### Exploit Title: Vehicle Service Management System - 'Multiple' Privilege Escalation Leads to CRUD Operations
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-46075
### CVSS: 7.2 HIGH
### References: 
- https://www.plsanu.com/vehicle-service-management-system-multiple-privilege-escalation-leads-to-crud-operations
- https://nvd.nist.gov/vuln/detail/CVE-2021-46075
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-46075

### Description:
A Privilege Escalation vulnerability exists in Sourcecodester Vehicle Service Management System 1.0. Staff account users can access the admin resources and perform CRUD Operations.

### 1. Vehicle Service Management System - 'User List' (/admin/?page=user/list) (/admin/?page=user/manage_user)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Login the Admin account in Browser A (Chrome)
3. Login the Staff account in Browser B (Firefox)
4. In Admin account(Chrome) navigate to the User List section and click on Create New button.
5. Copy the link of Create New User http://localhost/vehicle_service/admin/?page=user/manage_user
6. In Staff account (Firefox) there is no User List section because Staff Account didn't have the rights to create a user. 
7. Paste the copied link in Browser B (Firefox)
8. Now we are able to access the Create User Page.
9. Enter the required details and click on Save button.
10. In Admin account(Chrome) navigate to the User List section http://localhost/vehicle_service/admin/?page=user/list
11. The new user have been created successfully.
12. Staff account can able to access the User List page and Create User page.
13. Staff account can able to Create, Read, Update & Delete the users.

### 2. Vehicle Service Management System - 'Category List' (/admin/?page=maintenance/category) (/admin/?page=maintenance/manage_category)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Login the Admin account in Browser A (Chrome)
3. Login the Staff account in Browser B (Firefox)
4. In Admin account(Chrome) navigate to the Category List section and click on Create New button.
5. Copy the link of Create New Category http://localhost/vehicle_service/admin/?page=maintenance/manage_category
6. In Staff account (Firefox) there is no Category List section because Staff Account didn't have the rights to create a Category. 
7. Paste the copied link in Browser B (Firefox)
8. Now we are able to access the Create Category Page.
9. Enter the required details and click on Save button.
10. In Admin account(Chrome) navigate to the Category List section http://localhost/vehicle_service/admin/?page=maintenance/category
11. The new category have been created successfully.
12. Staff account can able to access the Category List page and Create Category page.
13. Staff account can able to Create, Read, Update & Delete the Category.

### 3. Vehicle Service Management System - 'Service List' (/admin/?page=maintenance/services) (/admin/?page=maintenance/manage_service)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Login the Admin account in Browser A (Chrome)
3. Login the Staff account in Browser B (Firefox)
4. In Admin account(Chrome) navigate to the Service List section and click on Create New button.
5. Copy the link of Create New Service http://localhost/vehicle_service/admin/?page=maintenance/manage_service
6. In Staff account (Firefox) there is no Service List section because Staff Account didn't have the rights to create a Service. 
7. Paste the copied link in Browser B (Firefox)
8. Now we are able to access the Create Service Page.
9. Enter the required details and click on Save button.
10. In Admin account(Chrome) navigate to the Service List section http://localhost/vehicle_service/admin/?page=maintenance/services
11. The new Serive have been created successfully.
12. Staff account can able to access the Service List page and Create Service page.
13. Staff account can able to Create, Read, Update & Delete the Service.

### 4. Vehicle Service Management System - 'Settings' (/admin/?page=system_info)

### Exploit:
1. Visit the admin panel http://localhost/vehicle_service/admin
2. Login the Admin account in Browser A (Chrome)
3. Login the Staff account in Browser B (Firefox)
4. In Admin account(Chrome) navigate to the Settings section.
5. Copy the link of Settings Section http://localhost/vehicle_service/admin/?page=system_info
6. In Staff account (Firefox) there is no Settings section because Staff Account didn't have the rights to access the Settings. 
7. Paste the copied link in Browser B (Firefox)
8. Now we are able to access the Settings page.
9. Enter the required details and click on Save button.
10. In Admin account(Chrome) navigate to the Settings section http://localhost/vehicle_service/admin/?page=system_info
11. The details have been updated successfully.
12. Staff account can able to access the Settings page.
13. Staff account can able to Update the Settings.

### Impact:
An remote attacker gain elevated access to resources that should normally be unavailable. In staff account users can able to access the admin resources and perform CRUD Operations.

### Mitigation:
It is recommended to implement the following:
- Proper privilege account management
- Monitor and manage all privileged sessions
- Strong password policies and enforcement
- Sanitize user inputs and secure the databases
