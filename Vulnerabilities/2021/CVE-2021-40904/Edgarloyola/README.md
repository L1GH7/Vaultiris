# CVE-2021-40904 - RCE via CheckMk's Dokuwiki embedded application

**Application:** CheckMK Management Web Console

**Software Revision:** From 1.5.0 to 1.5.0p25

**Attack type:** RCE

**Solution:** Upgrade to version 1.6 or higher.

**Summary:** The web management console of CheckMk Raw Edition (versions 1.5.0 to 1.5.0p25) allows a misconfiguration of the web-app Dokuwiki (installed by default) which allows embedded php code. As a result, remote code execution is achieved. Successful exploitation requires access to the web management interface, either with valid credentials or with a hijacked session by a user with the role of administrator.

**Technical Description:** See CVE-2021-40904

**Timeline:**
   * 2021-09-01 Issues discovered.
   * 2021-09-06 First contact with vendor via e-mail.
   * 2021-09-08 Vendor response. RCE vulnerabilities were already detected, and higher versions already exist with the patch.
   * 2022-03-25 Public disclosure.
  

**Reference:**
   * https://checkmk.com/download/archive#checkmk-1.6.0
   * https://checkmk.com/download/archive#checkmk-2.0.0
   * http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-40904
   * https://nvd.nist.gov/vuln/detail/CVE-2021-40904

# DEMO
  **PoC checkmk version 1.5.0p25 Raw Edition**
  
  ![RCE Dokuwiki](https://raw.githubusercontent.com/Edgarloyola/RCE_checkMK_Dokuwiki/main/RCE_Dokuwiki.png?token=AKM5DAKN4PSOWFSDA35RNMDBMVMWO)
