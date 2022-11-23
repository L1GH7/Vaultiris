# CVE-2021-42949
The component controlla_login function in HotelDruid Hotel Management Software v3.0.3 generates a predictable session token, allowing attackers to bypass authentication via bruteforce attacks.

The session id is dynamically created at each login using the following generalized syntax: {date}{time}{100000-999999}{incremented login attempts}.  These values can be guessed and/or brute forced relatively quickly.  The session id is correlated with other data stored (remote_addr and user-agent) which partially mitigates this issue; however, the user agent can be guessed/known and the remote_addr can by bypassed by using the same computer as the vulnerable session and/or irrelevant if, for example, the webapp is behind a reverse proxy.

To exploit the vulnerability, an attacker may identify a valid session id and gain authenticated access through a combination of guessed information and brute force.

## Remediation
The vendor has committed to patching this vulnerability in the next release cycle of their software.  Hotel Druid administrators should update their software to the patched version once made available.

## Reference
https://www.hoteldruid.com/

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-42949
