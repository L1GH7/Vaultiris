## CVE-2022-39196
###  Black board CMS Escalation of Privileges

### Blackboard Learn version 1.10.1 allows remote authenticated users to read unintended files by entering student credentials and then directly visiting a certain webapps/bbcms/execute/ URL.

### Additional Information
	Step 1: Use a student credentials privilege. Username: ********** & Password: **********, to login. 
	Step 2: After successfully login by "STUDENT" account privilege.
	Step 3: Then request "URL / Location of vulnerability".
	Step 4: Directly without any privilege requirements you will escalated the session from "Student" to "Contest Management System" privileges.

### Vulnerability Type
	Sensitive Data Exposure

### Vendor of Product
	Blackboard Learn LMS


### Affected Product Code Base
	LMS - 1.10.1
	CMSMAIN - 1.10.1

 
### Attack Type
	Remote
 

### Impact Escalation of Privileges
	true


### Impact Information Disclosure
	true

### Attack Vectors
	Impact 1: View systems directories such as courses , institution, library and orgs directories & its contains.
 	Impact 2: Basic & Advance searching over courses , institution, library and orgs directories.
 	Impact 3: Searching & view about Blackboard LMS institution users.
 	Impact 4: Download files.

### Reference
	https://drive.google.com/drive/folders/1gonDDt0sCkpMdPDu_ZVwZ7EfLC8Z4JVn?usp=sharing

### Has vendor confirmed or acknowledged the vulnerability?
	true
### Discoverer
	Waseem Dayili
 
