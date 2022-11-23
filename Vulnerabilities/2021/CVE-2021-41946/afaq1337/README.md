# CVE-2021-41946

# Author: ``M. Afaq Abid``

PoC of CVE-2021-41946 - FiberHome VDSL2 Modem HG150-Ub_V3.0 (PTCL)

## Stored Cross-site scripting (XSS) vulnerability in Parental Control.


## Steps to Reproduce:

1. Go to login page of Modem (192.168.10.1)
2. Advanced Setup --> Parental Control --> Access Time Restriction --> Username field
3. Add this Payload:``"><svg/onload=confirm(1);>`` in ``UserName`` field and add a rule.
4. Go now Parental Control section and Stored XSS will executed. 

Stored XSS prevent user to delete the rule now and this could be abused by an attacker to add restriction through Parental Control on a user and prevent them to delete that.

(Hard Reset Required to delete the Stored XSS)

PoC video for more details: https://youtu.be/mE46eNtyFIo
