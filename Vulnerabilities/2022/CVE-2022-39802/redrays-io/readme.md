# 3242933 – [CVE-2022-39802] File path traversal vulnerability in SAP Manufacturing Execution

## General information
Risk: CRITICAL

Versions Affected:
```
SAP MFG EXECUTION CORE 15.1
SAP MFG EXECUTION CORE 15.2
SAP MFG EXECUTION CORE 15.3
```

Vendor URL: https://sap.com

Bug: File path traversal vulnerability

Reported: September 2022

Date of Public Advisory: October 12, 2022

Reference: SAP Security Note  3242933

Author: RedRays R&D

## Description
ADVISORY INFORMATION
Title: 3242933 – [CVE-2022-39802] File path traversal vulnerability in SAP Manufacturing Execution
Advisory: [RedRays-22-031]

## VULNERABILITY INFORMATION
Remotely Exploitable: Yes
Locally Exploitable: No
  
CVSS Information
CVSS v3 Base Score: 9.9 / 10 (AV:N/AC:L/PR:L/UI:N/S:C/C:H/I:H/A:N)

## VULNERABILITY DESCRIPTION
With a CVSS rating of 9.9, the vulnerability fixed in SAP Security Note #3242933 affects SAP Manufacturing Execution and is considered significant.

The URL used to inquire for this data included a file path argument that could be modified to provide unrestricted directory browsing on the remote server. The OS user running the NetWeaver process or service would be able to access the files contained in each directory. The code fix in the patch takes care of the route internally. This prevents the value from being sent in dynamically as a query string. With a CVSS score of 9.9, the effect on confidentiality, integrity, and availability may be quite substantial depending on the kind of information that is accessed during an attack.

SAP suggests a temporary fix in which sensitive data is removed from the file systems available to the OS user and the OS user’s access to unnecessary file paths is restricted.

## TECHNICAL DESCRIPTION

PoC will be published in January 2023.

The signature of the vulnerability is available on the RedRays Security Platform.
[RedRays Advisory](https://redrays.io/3242933-cve-2022-39802-file-path-traversal-vulnerability-in-sap-manufacturing-execution/)
## ABOUT RedRays
The main goal of RedRays is to narrow the security gap in both the technical and business realms. The company offers solutions to examine and protect SAP, Oracle, and Microsoft ERP systems against cyberattacks and internal fraud.

Typically, our customers are big organizations and managed service providers whose needs include the active monitoring and management of security across extensive SAP environments worldwide.

## ABOUT RedRays R&D
The company’s competence is founded on RedRays’ research section, which specializes in vulnerability research and analysis of essential corporate applications. It has received several accolades from major software companies, including SAP, Oracle, Microsoft Dynamics, and IBM.

Experts from RedRays have been asked to lecture, present, and train at major international security conferences on all continents.



Address: Casablanca, Morocco

Phone: (+32)489-16-78-85
