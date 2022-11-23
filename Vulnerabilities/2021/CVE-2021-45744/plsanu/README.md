# CVE-2021-45744

### Exploit Title: Bludit 3.13.1 - TAGS Field Stored Cross Site Scripting (XSS)
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-45744
### CVSS: 5.4 MEDIUM
### References: 
- https://www.plsanu.com/bludit-3-13-1-tags-field-stored-cross-site-scripting-xss
- https://nvd.nist.gov/vuln/detail/CVE-2021-45744
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-45744

### Description:
A Stored Cross Site Scripting (XSS) vulnerability exists in bludit 3.13.1 via the TAGS section in login panel. Application stores attacker injected dangerous JavaScript in to the database and executes without validating.

### Exploit:
1. Login to the admin panel - http://localhost/admin
2. Navigate to New content section.
3. Enter the title of the post Ex:test
4. Click on the Options button.
5. Navigate to Advanced tab and inject the below payload in TAGS section.

### Payload:
```html
"><script>alert("XSS")</script>
```

6. Click on Save button.
7. Open the post(test).
8. Malicious javascript code triggered.

### Impact:
An attacker can able to inject malicious JavaScript code in TAGS Field Section.

### Mitigation:
It is recommended to sanitize all the input fields throughout the application.
