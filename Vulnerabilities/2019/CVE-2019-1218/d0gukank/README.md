# CVE-2019-1218 Outlook iOS Spoofing Vulnerability


I discovered XSS in IOS Outlook Mobile Application. A severe vulnerability in Microsoft Outlook for IOS that could allow an attacker to takeover a victim’s device. The security issue could enable a malicious actor to execute a cross-site scripting (XSS) exploit on any IOS smartphone or tablet using the email client. This is due to the way the software parses specifically crafted email messages.

By using the Javascript payloads, It can be modified content on the application.

A spoofing vulnerability exists in the way Microsoft Outlook iOS software parses specifically crafted email messages. An authenticated attacker could exploit the vulnerability by sending a specially crafted email message to a victim.

The attacker who successfully exploited this vulnerability could then perform cross-site scripting attacks on the affected systems and run scripts in the security context of the current user.

The security update addresses the vulnerability by correcting how Outlook for IOS parses specially crafted email messages.

https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/CVE-2019-1218




[![poc.gif](https://s5.gifyu.com/images/poc.gif)](https://gifyu.com/image/vcEB)
