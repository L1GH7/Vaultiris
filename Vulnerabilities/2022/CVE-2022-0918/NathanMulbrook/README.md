# CVE-2022-0918

Red Hat CVE page: https://access.redhat.com/security/cve/cve-2022-0918

Github issue with patches linked: https://github.com/389ds/389-ds-base/issues/5242

## PoC

The PoC application takes in a single IP, either IPv4 or IPv6 will work. <br />
By default it will send the message to port 389, this can be changed in the source. <br />
The PoC will make a TCP connection to the target and transmit a single packet, the target slapd should then seg fault.

### Useage
```bash
clang poc.c -o poc
./poc [IP Address]
```
