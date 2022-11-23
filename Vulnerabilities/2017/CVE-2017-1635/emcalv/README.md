# tivoli-poc

CVE-2017-1635 PoC code

CVEID: CVE-2017-1635

CVSS Base Score: 8

Affected Products and Versions: KDH component of IBM Tivoli Monitoring Basic Services (KGL,KAX) for Version 6.2.2.0 through 6.2.2.9
 

A vulnerability exists in the internal web server provided by IBM Tivoli Monitoring basic services. It could allow a remote attacker to execute arbitrary code on the system, caused by a use-after-free error. A remote attacker could exploit this vulnerability to execute arbitrary code on the system or cause the application to crash.
The web server component "KDH", after receiving certain requests, executes a memory region in the heap previously freed by the component itself.
An attacker is able to fill the heap before the memory is reused, in order to execute arbitrary code.

http://www-01.ibm.com/support/docview.wss?uid=swg22010554

https://www.securityfocus.com/bid/101905

http://www.quantumleap.it/ibm-tivoli-monitoring-cve-2017-1635-remote-code-execution-vulnerability/

