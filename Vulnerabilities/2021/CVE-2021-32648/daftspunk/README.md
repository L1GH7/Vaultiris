# CVE-2021-32648

Patch your code for October CMS Auth Bypass CVE-2021-32648

# Instructions

1. Open the file **vendor/october/rain/src/Auth/Models/User.php**
2. [Perform the patch found in these diff notes](https://github.com/daftspunk/CVE-2021-32648/commit/7dc2ce8b6d64a1954089aece560ef9f3e319b7a9)
3. Save the file

# Overview

You are converting a loose comparison to a strict comparison by replacing two (2) equal signs `==` with three (3) equal signs `===`. This blocks the attack vector as described in [CVE-2021-32648](https://github.com/octobercms/october/security/advisories/GHSA-mxr5-mc97-63rc) and also [CVE-2021-29487](https://github.com/octobercms/october/security/advisories/GHSA-h76r-vgf3-j6w5).

This issue has been patched in October CMS Build 472 (v1.0.472+) and v1.1.5+. This issue does not affect v2.0.0+.
