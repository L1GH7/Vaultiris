# adaPwn - CVE-2019-14912 PoC

adAS OPENSSO module doesn't correctly verify the domain to redirect, making possible to redirect the user to an attacker controlled website, stealing his adAS session cookie.

# Usage

``
usage: adaPwn.py [-h] [--interface INTERFACE] [--httpPort HTTPPORT]
                 [--dnsPort DNSPORT]
                 ipToSpoof realIP redirectUrl ipBlacklist [ipBlacklist ...]``
