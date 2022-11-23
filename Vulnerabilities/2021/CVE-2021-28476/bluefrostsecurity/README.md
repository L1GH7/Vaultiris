# CVE-2021-28476: Microsoft Hyper-V: Multiple Vulnerabilities in vmswitch
Multiple bugs are present in WPP code when handling set OID requests, one of them allowing to dereference (read access) an attacker controlled pointer, and the rest causing out of bounds read accesses. This repository contains the proof-of-concept code to trigger the identified issues.

A full technical description of the issues can be found at https://labs.bluefrostsecurity.de/advisories/bfs-sa-2021-001/
