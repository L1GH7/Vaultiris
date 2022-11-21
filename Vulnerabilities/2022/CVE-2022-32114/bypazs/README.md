# Strapi v4.1.12

### Vulnerability Explanation:
An unrestricted file upload vulnerability in the Add New Assets function of Strapi v4.1.12 allows attackers to execute arbitrary code via a crafted file

### Attack Vectors:
- After uploading a file containing malicious content, when the user opens the link to the file, it will execute.

### Payload :
https://github.com/bypazs/GrimTheRipper/blob/main/GrimTheRipperTeam.pdf

### Tested on:
1.  Strapi Version 4.1.12
2.  Google Chrome Version 102.0.5005.61 (Official Build) (64-bit)

### Affected Component: 
- On the Media Library page, it is allowed to upload files containing malicious content to the system.

### Steps to attack:
1. Log in with a user that has permission to upload files.
2. Click on the "Media Library" menu, then click on "+ Add new assets".
3. Click on the "Browse files: button, and then select the prepared file containing malicious content.
4. Then click on the "Upload 1 asset to the library" button to upload the file to the system.
5. Click edit in the corner of the file and click copy link.
6. Paste the link to a new tab, it will show that the payload XSS was executed.

### Discoverer:
:shipit: Grim The Ripper Team by SOSECURE Thailand

### Medium:
- https://grimthereaperteam.medium.com/strapi-v4-1-12-unrestricted-file-upload-b993bfd07e4e

### Disclosure Timeline:
- 2022–05–29: Vulnerability discovered.
- 2022–05–29: Vulnerability reported to the MITRE corporation.
- 2022–07–14: CVE has been reserved.
- 2022–05–29: Public disclosure of the vulnerability.

Reference:
1. https://github.com/strapi/strapi
2. https://strapi.io/
3. https://github.com/bypazs/strapi
4. https://grimthereaperteam.medium.com/strapi-v4-1-12-unrestricted-file-upload-b993bfd07e4e

