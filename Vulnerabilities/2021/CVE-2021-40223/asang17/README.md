# CVE-2021-40223
**Application**: Rittal CMC PU III Web management

**Devices**: CMC PU III 7030.000

**Software Revision**: V3.11.00_2

**Hardware Revision**: V3.00

**Attack type**: Stored XSS

**Solution**: Update to Software Revision V3.17.10 or later

**Summary**: Web application fails to sanitize user input on Security User configuration dialog and Task tab. This allows attacker to inject HTML or browser interpreted content in the web application. In this case, the XSS of the user configuration will be displayed when the authentication is performed and also in the logs. The XSS of the task will also be interpreted in the log section. It is interesting to remark that both XSS will be persistent in the logs until they are deleted, even if the rogue input values are changed to correct ones. Successful exploitation requires access to the web management interface with a valid or hijacked session.

**Timeline**:
* 2021-08-03 Issues discovered
* 2021-08-08 First contact with vendor via e-mail
* 2021-08-23 Second contact with vendor via e-mail
* 2021-09-01 Vulnerability patch confirmed
