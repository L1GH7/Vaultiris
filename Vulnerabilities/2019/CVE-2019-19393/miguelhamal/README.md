# Rittal CMC PU III – Stored XSS PoC

**Application:** Rittal CMC PU III Web management

**Devices:** CMC PU III 7030.000

**Software Revision:** From V3.11.00_2 to V3.15.70_4

**Hardware Revision:** From V3.00 to V6.01

**Attack type:** Stored XSS

**Solution:** Update to Software Revision V3.17.10 or later

**Summary:** Web application fails to sanitize user input on system configurations page. This allows attacker
to backdoor the device with HTML and browser interpreted content (such as JS or other client-side scripts) as
the content is displayed always after and before login. Persistent XSS allows attacker to modify displayed
content or to change the victim's information. Successful exploitation requires access to the web management
interface either with valid credentials or hijacked session.

**Technical Description:** See CVE-2019-19393.pdf

**Timeline:**
   * 2019-11-11 Issues discovered
   * 2019-11-28 First contact with vendor via e-mail
   * 2020-03-02 Second contact with vendor via e-mail
   * 2020-03-02 Vendor response. XSS vulnerabilities were already detected, and would be patched in the next release
   * 2020-08-20 New Software Version release. V3.17.10   
   * 2020-09-28 Vulnerability patch confirmed   
