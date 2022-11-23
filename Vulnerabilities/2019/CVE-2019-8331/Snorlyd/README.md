# https-nj.gov---CVE-2019-8331
#### Vulnearability Report of the New Jersey official site
Bootstrap 4.0.0	Found in https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js _____Vulnerability info:

Medium	28236 XSS in data-template, data-content and data-title properties of tooltip/popover CVE-2019-8331	

Versions of bootstrap prior to 3.4.1 for 3.x and 4.3.1 for 4.x are vulnerable to Cross-Site Scripting (XSS). The data-template attribute of the tooltip and popover plugins lacks input sanitization and may allow attacker to execute arbitrary JavaScript.

# RECOMMENDATION
For bootstrap 4.x upgrade to 4.3.1 or later.
For bootstrap 3.x upgrade to 3.4.1 or later.

# REFERENCES
https://nvd.nist.gov/vuln/detail/CVE-2019-8331
https://blog.getbootstrap.com/2019/02/13/bootstrap-4-3-1-and-3-4-1/
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-8331
GHSA-9v3m-8fp8-mj99
https://www.npmjs.com/advisories/891
twbs/bootstrap#28236
https://access.redhat.com/errata/RHSA-2019:1456
https://access.redhat.com/errata/RHSA-2019:3023
https://access.redhat.com/errata/RHSA-2019:3024
https://github.com/twbs/bootstrap/releases/tag/v3.4.1
https://github.com/twbs/bootstrap/releases/tag/v4.3.1
https://lists.apache.org/thread.html/10f0f3aefd51444d1198c65f44ffdf2d78ca3359423dbc1c168c9731@%3Cdev.flink.apache.org%3E
https://lists.apache.org/thread.html/17ff53f7999e74fbe3cc0ceb4e1c3b00b180b7c5afec8e978837bc49@%3Cuser.flink.apache.org%3E
https://lists.apache.org/thread.html/519eb0fd45642dcecd9ff74cb3e71c20a4753f7d82e2f07864b5108f@%3Cdev.drill.apache.org%3E
https://lists.apache.org/thread.html/52bafac05ad174000ea465fe275fd3cc7bd5c25535a7631c0bc9bfb2@%3Cuser.flink.apache.org%3E
https://lists.apache.org/thread.html/52e0e6b5df827ee7f1e68f7cc3babe61af3b2160f5d74a85469b7b0e@%3Cdev.superset.apache.org%3E
https://lists.apache.org/thread.html/54df3aeb4239b64b50b356f0ca6f986e3c4ca5b84c515dce077c7854@%3Cuser.flink.apache.org%3E
https://lists.apache.org/thread.html/b0656d359c7d40ec9f39c8cc61bca66802ef9a2a12ee199f5b0c1442@%3Cdev.drill.apache.org%3E
https://lists.apache.org/thread.html/f9bc3e55f4e28d1dcd1a69aae6d53e609a758e34d2869b4d798e13cc@%3Cissues.drill.apache.org%3E
https://lists.apache.org/thread.html/r3dc0cac8d856bca02bd6997355d7ff83027dcfc82f8646a29b89b714@%3Cissues.hbase.apache.org%3E
https://lists.apache.org/thread.html/rd0e44e8ef71eeaaa3cf3d1b8b41eb25894372e2995ec908ce7624d26@%3Ccommits.pulsar.apache.org%3E
https://seclists.org/bugtraq/2019/May/18
https://support.f5.com/csp/article/K24383845
https://support.f5.com/csp/article/K24383845?utm_source=f5support&amp;utm_medium=RSS
https://www.oracle.com/security-alerts/cpuApr2021.html
http://packetstormsecurity.com/files/156743/OctoberCMS-Insecure-Dependencies.html
http://seclists.org/fulldisclosure/2019/May/10
http://seclists.org/fulldisclosure/2019/May/11
http://seclists.org/fulldisclosure/2019/May/13
http://www.securityfocus.com/bid/107375
