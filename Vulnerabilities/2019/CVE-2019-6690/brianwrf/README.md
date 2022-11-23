# Summary

It is a simple PoC of Improper Input Validation in python-gnupg 0.4.3 (CVE-2019-6690).

# Vulnerable

* python python-gnupg 0.4.3
* python python-gnupg 0.3.6
* python python-gnupg 0.3.5
* python python-gnupg 0.3.4 

# Mitigation

Users should upgrade to 0.4.4

# Test Environment

## Docker Image

* docker pull avfisherdocker/python-gnupg0.4.3:CVE-2019-6690
* docker run -d -p 5000:5000 avfisherdocker/python-gnupg0.4.3:CVE-2019-6690

# Proof of Concepts

1. `apt install libmojolicious-perl`
2. `git clone https://github.com/brianwrf/CVE-2019-6690`
3. `cd CVE-2019-6690`
4. `perl exploit.pl <ip> <port>`

# Reference

* <https://github.com/stigtsp/CVE-2019-6690-python-gnupg-vulnerability>
* <https://seclists.org/bugtraq/2019/Jan/41>
* <https://www.securityfocus.com/bid/106756>
