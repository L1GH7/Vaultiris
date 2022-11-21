# CVE-2022-36200

# Author: ``M. Afaq Abid``

PoC of CVE-2022-36200. - FiberHome VDSL2 Modem HG150-Ub_V3.0 (PTCL)

## Information Exposure

## Steps to Reproduce:

1. Admin Credentials are submitted in the URL.
2. Use any logger/sniffer in network (wireshark)
3. Search `username` & `password` params in `login.cgi?`

These could be captured by anyone in the network as these are submitted over `http` (not encrypted) and these might be logged in network logs and can be sniffed as well.

PoC video for more details: https://youtu.be/nHgstvq0rr8

