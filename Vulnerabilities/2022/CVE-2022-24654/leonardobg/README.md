# CVE-2022-24654

PoC of CVE-2022-24654 - INTELBRAS ATA 200 Firmware 74.19.10.21

## Authenticated stored Cross Site Scripting


## Steps to Reproduce:

1. Log in the equipment via your web browser
2. Go to Management > Syslog
3. In the "Field Server Address" inject the payload "-prompt("XSS")-"
4. Click Save
5. Exploit!
