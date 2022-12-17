# CVE-2021-38699 TastyIgniter 3.0.7 allows XSS via the name field during user-account creation.


A Stored Cross Site Scripting Vulnerability exists in multiple pages of TastyIgniter v3.0.7 that allows for arbitrary execution of JavaScript. https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-38699

Vulnerable Pages:
/account, /reservation, /admin/dashboard, /admin/system_logs

Vulnerable Payloads:
“><script> alert(1) </script> <script> alert(1) </script>


Found by Justin White and Matt Kiely | HuskyHacks, August 2021
