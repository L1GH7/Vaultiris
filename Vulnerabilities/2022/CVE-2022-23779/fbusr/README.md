# CVE-2022-23779: Internal Hostname Disclosure Vulnerability
## Information
**Description:** Zoho ManageEngine Desktop Central before 10.1.2137.8 exposes the installed server name to anyone. The internal hostname can be discovered by reading HTTP redirect responses.  
**Versions Affected:** <10.1.2137.7 \
**Researcher:** Matthew Zellner (@fbusr)  
**Disclosure Link:** https://www.manageengine.com/products/desktop-central/cve-2022-23779.html  
**NIST CVE Link:** https://nvd.nist.gov/vuln/detail/CVE-2022-23779  

## Proof-of-Concept Exploit
### Description
1. Send an HTTP GET request to '/themes' over port 8020.
2. Read the HTTP redirect response and anaylze the 'Location' HTTP response header.  

### Usage/Exploitation
`./CVE-2022-23779.sh`
