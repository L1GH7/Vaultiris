Evaluates hosts for CVE-2014-0224 vulnerability
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2014-0224

Usage:

>ccs-eval.py list-of-hosts.txt

-Takes in a list of hosts, line seperated. Checks the host for common SSL ports using 
nmap. Peforms PoC injection test supplied by RedHat (fake-client-early-ccs.pl). Writes 
results to "local-results-list-of-hosts.txt"

