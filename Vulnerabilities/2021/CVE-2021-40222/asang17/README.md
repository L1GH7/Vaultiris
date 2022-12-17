# CVE-2021-40222
**Application**: Rittal CMC PU III Web management

**Devices**: CMC PU III 7030.000

**Software Revision**: V3.11.00_2

**Hardware Revision**: V3.00

**Attack type**: Remote Code Execution

**Solution**: Update to Software Revision V3.17.10 or later

**Summary**: Web application fails to sanitize user input on Network TCP/IP configuration page. This allows the attacker to inject commands as root on the device which will be executed once the data is received after a few seconds. An attacker can create a backdoor in the device or just execute a reverse shell which connects to the attacker machine. Successful exploitation requires admin access to the management of the device with a valid or hijacked session.

**Timeline**:
* 2021-08-03 Issues discovered
* 2021-08-08 First contact with vendor via e-mail
* 2021-08-23 Second contact with vendor via e-mail
* 2021-09-01 Vulnerability patch confirmed
