# CVE-2022-0219

Jadx is vulnerable to Improper Restriction of XML External Entities (CWE-611) when exporting a malicious APK file (via the -e option) containing a crafted AndroidManifest.xml / strings.xml to gradle. 

It can lead to local file disclosure when using it from the CLI. When used as a library, it can lead to local file disclosure / denial of service if the APK file can come from an untrusted source. The GUI is not known to be affected.

Original report: https://huntr.dev/bounties/0d093863-29e8-4dd7-a885-64f76d50bf5e/

Fixed in [jadx-1.3.2](https://github.com/skylot/jadx/releases/tag/v1.3.2)
