# CVE-2020-26061
# ClickStudios Passwordstate Password Reset Portal Authentication Bypass

## Information
**Vendor:** ClickStudios  
**Product:** Passwordstate  
**Versions Affected:** Before 8.5 build 8501  
**Researcher:** Jason Juntunen aka @missing0x00 (https://github.com/missing0x00)  

## Description
ClickStudios Passwordstate Password Reset Portal prior to 8.5 build 8501 is affected by an authentication bypass vulnerability. The ResetPassword function does not validate whether the user has successfully authenticated using security questions. An unauthenticated, remote attacker can send a crafted HTTP request to the /account/ResetPassword page to set a new password for any registered user.

### Proof of Concept
https://github.com/missing0x00/CVE-2020-26061/blob/main/CVE-2020-26061.py

### Remediation
Update the software on all affected devices. This vulnerability was fixed in version 8.5 - Build 8501 (October 12, 2018).

## References
**Vendor Link:** https://www.clickstudios.com.au/passwordstate-changelog.aspx  
**Disclosure Link:** https://github.com/missing0x00/CVE-2020-26061  
**NIST CVE Link:** https://nvd.nist.gov/vuln/detail/CVE-2020-26061  

## Disclaimer
The information contained within this advisory is supplied "as-is" with no warranties or guarantees of fitness of use or otherwise. Permission is hereby granted for the redistribution of this advisory, provided that it is not altered except by reformatting it, and that due credit is given. Permission is explicitly given for insertion in vulnerability databases and similar, provided that due credit is given to the author. The author is not responsible for any misuse of the information contained herein and accepts no responsibility for any damage caused by the use or misuse of this information. The author prohibits any malicious use of security related information or exploits by the author or elsewhere.

missingnull
