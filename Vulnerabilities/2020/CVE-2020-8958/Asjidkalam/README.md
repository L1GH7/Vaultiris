# CVE-2020-8958
**CVE-2020-8958**: Authenticated Remote Code Execution Exploit for NetLink Routers using `boa` server.
<br><br>
<b>CVSS Score:</b> 7.2
<br>
<b>Vulnerability Type(s):</b> OS Command Injection
<br>
<b>Authentication:</b> Required
<br>
<b>Affected Model(s):</b> HG323
<br>

# Description
The `/boaform/admin/formPing` resource in Netlink routers allows remote attackers to perform `OS Command Injection` via the `target_addr` parameter.

# Usage
```bash
    usage: CVE-2020-8958.py [-h] -i URL [-u [USER]] [-p [PASS]]

    CVE-2020-8958: Authenticated remote code execution exploit

    optional arguments:
    -h, --help            show this help message and exit
    -i URL, --Url URL     Target IP of router
    -u [USER], --User [USER]
                            Username
    -p [PASS], --Pass [PASS]
                            Password
```
