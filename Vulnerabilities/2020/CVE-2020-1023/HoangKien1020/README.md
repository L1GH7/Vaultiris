# Made by HK
# CVE-2020-10238: Incorrect Access Control in com_templates- RCE
# CVE-2020-10239: Incorrect Access Control in com_fields SQL field- RCE
# Link
https://developer.joomla.org/security-centre/804-20200303-core-incorrect-access-control-in-com-templates.html
https://developer.joomla.org/security-centre/806-20200305-core-incorrect-access-control-in-com-fields-sql-field.html
# My blog about this CVE
https://hoangkien1020.tech/index.php/2020/03/13/my-journey-to-find-out-joomlas-cvepart-1/

# Guide to use docker such as:
# #Step 1: 

# *docker pull hoangkien1020/joomla:hk*

# #Step 2:

# *docker run -d --rm -it -p 8080:80 hoangkien1020/joomla:hk*

# #Step 3: Access your domain/IP with port 8080:
![image](https://user-images.githubusercontent.com/24661746/75947931-9be86d80-5ed4-11ea-991d-f37309d4c41a.png)
# Inside this image with credentials

### *username: password*

### MySQL: root: root (can access via IP:8080/phpmyadmin)

### superadmin:1234 (Super Users)

### admin:1234 (Administrator)

### hacker:1234 (Manager) 
