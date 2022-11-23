# CVE-2021-45745

### Exploit Title: Bludit 3.13.1 - About Plugin Stored Cross Site Scripting (XSS)
### Exploit Author: <a href="https://www.plsanu.com">P.L.Sanu</a>
### CVE: CVE-2021-45745
### CVSS: 5.4 MEDIUM
### References: 
- https://www.plsanu.com/bludit-3-13-1-about-plugin-stored-cross-site-scripting-xss
- https://nvd.nist.gov/vuln/detail/CVE-2021-45745
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-45745

### Description:
A Stored Cross Site Scripting (XSS) vulnerability exists in Bludit
3.13.1 via the About Plugin in login panel. Application stores attacker injected dangerous JavaScript in to the database and executes without validating.

### Exploit:
1. Login to the admin panel http://localhost/admin
2. Navigate to Themes section.
3. Activate the Blog X theme.
4. Navigate to plugins section.
5. In About plugin click the Settings button.
5. Inject the below payload in About section.

### Payload:
```html
"><script>alert("XSS")</script>
```

7. Click on Save button.
8. Visit the site.
9. Malicious javascript code triggered.

### Impact:
An attacker can able to inject malicious JavaScript code in About Plugin.

### Mitigation:
It is recommended to sanitize all the input fields throughout the application.
