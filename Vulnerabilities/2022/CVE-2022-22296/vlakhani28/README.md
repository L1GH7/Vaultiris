# CVE-2022-22296
## All Details about CVE-2022-22296

Software: Hospital's Patient Records Management System 1.0 

Software Link: https://www.sourcecodester.com/php/15116/hospitals-patient-records-management-system-php-free-source-code.html

Vulnerability Type: Insecure Permissions - IDOR

Affected Component: id parameter in Change User Function

Impact Escalation of Privileges: true

Attack Type: Remote

Vendor of Product: Sourcecodester

### Description: 
<hr />
Insecure direct object references (IDOR) are a type of access control vulnerability that arises when an application uses user-supplied input to access objects directly. The vulnerability exists in Sourcecodester Hospital's Patient Records Management System Website 1.0 via the id parameter in manage_user endpoint. Simply change the value and data of other users can be displayed.
The URL would look like: http://localhost/hprms/admin/?page=user/manage_user&id=3 where the "id" parameter is vulnerable

Impact: This vulnerability allows an attacker to edit information that do not belong to him and remove them from the users account.
