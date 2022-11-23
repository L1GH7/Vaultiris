# CVE-2021-36563 – Multiple Stored and Reflected XSS

**Application:** CheckMK Management Web Console

**Software Revision:** From 1.5.0 to 2.0.0p9

**Attack type:** Stored and Reflected XSS

**Solution:** Update to Software Revision 2.0.0p10 or later

**Summary:** The CheckMK management web console (versions 1.5.0 to 2.0.0) does not sanitise user input in various parameters of the WATO module. This allows an attacker to open a backdoor on the device with HTML content and interpreted by the browser (such as JavaScript or other client-side scripts), the XSS payload will be triggered when the user accesses some specific sections of the application. In the same sense a very dangerous potential way would be when an attacker who has the monitor role (not administrator) manages to get a stored XSS to steal the secretAutomation (for the use of the API in administrator mode) and thus be able to create another administrator user who has high privileges on the CheckMK monitoring web console. Another way is that persistent XSS allows an attacker to modify the displayed content or change the victim's information. Successful exploitation requires access to the web management interface, either with valid credentials or with a hijacked session.

**Technical Description:** See CVE-2021-36563

**Timeline:**
   * 2021-04-12 Issues discovered.
   * 2021-04-26 First contact with vendor via e-mail.
   * 2021-04-28 Second contact with vendor via e-mail.
   * 2021-04-29 Vendor response. XSS vulnerabilities were already detected, and would be patched in the next release.
   * 2021-05-07 Vendor publicly exposes the vulnerability and its patch.
   * 2021-05-12 New Software Version release(2.0.0p4) and Vulnerability patch confirmed.
   * 2021-07-26 Public disclosure.
   * 2021-07-28 New issues discovered and contact with the supplier by e-mail.
   * 2021-07-29 Vendor response. XSS vulnerabilities were already detected, and would be patched in the next release.
   * 2021-08-26 Vendor publicly exposes the vulnerability and its patch.
   * 2021-09-16 New Software Version release(2.0.0p11) and Vulnerability patch confirmed.

**Reference:**
   * https://checkmk.com/de/werk/12762
   * https://checkmk.com/de/werk/13148
   * https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-36563
   * https://nvd.nist.gov/vuln/detail/CVE-2021-36563

# DEMO
  **PoC checkmk version 1.5.0p19 Raw Edition**
  
[![PoC checkmk version 1.5.0p19 Raw Edition](https://j.gifs.com/w0VP7m.gif)](https://youtu.be/-8ZTxkmxlIg)

  **PoC checkmk version 2.0.0p1 Enterprise Edition**
  
[![PoC checkmk version 2.0.0p1 Enterprise Edition](https://j.gifs.com/k28zXY.gif)](https://youtu.be/S15b2lN5YQE)
