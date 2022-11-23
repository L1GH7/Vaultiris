# Foxit Reader - CPDF_Parser::m_pCryptoHandler - Use After Free - RCE

# Vulnerability

Referencing memory after it has been freed can cause a program to crash, use unexpected values, or execute code


# Vulnerability Description

This vulnerability allows remote attackers to execute arbitrary code on vulnerable installations of Foxit Reader. User interaction is required to exploit this vulnerability in that the target must visit a malicious page or open a malicious file.

The specific flaw exists within the handling of **CPDF_Parser::m_pCryptoHandler**. The issue results from the lack of validating the existence of an object prior to performing operations on the object. An attacker can leverage this vulnerability to execute code under the context of the current process.


# CVE ID

CVE-2018-14442


# Vendor

www.foxitsoftware.com


# Product

* Foxit Reader 9.0.1.1049 and prior
* Foxit PhantomPDF 9.0.1.1049 and prior


# Disclosure Timeline

1. 08 January 2018 - Reported to vendor
2. 16 August 2018 - Coordinated public release of advisory


# Exploit

https://github.com/payatu/CVE-2018-14442


# Credits

Sudhakar Verma and Ashfaq Ansari - Project Srishti


# Vendor Advisory

https://www.foxitsoftware.com/support/security-bulletins.php
