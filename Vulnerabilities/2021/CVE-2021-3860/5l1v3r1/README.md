# CVE-2021-38601
CVE-2021-38601 - Reflected XSS in Pepperminty-Wiki 0.23-dev

A reflected Cross Site Scripting (XSS) vulnerability exists on multiple parameters in version 0.23-dev of the Pepperminty-Wiki application that allows for arbitrary execution of JavaScript commands.

Affected page: http://localhost/index.php

Sample payload: [https://localhost/index.php?action=<script>alert(1)</script>](https://localhost/index.php?action=<script>alert(1)</script>)

CVE: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-38601

Discovered by Heath Adams & Joe Helle, August 2021
