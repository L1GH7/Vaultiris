## Exploit Information

**Exploit Title:** Wowza Streaming Engine 4.8.11+5 - Denial of Service  
**CVE:** [CVE-2021-35492](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-35492)  
**Date:** 2021-10-06  
**Exploit Author:** N4nj0  
**Software Link:** [https://www.wowza.com/products/streaming-engine](https://www.wowza.com/products/streaming-engine)  
**Version:** 4.8.11+5  
**Tested on:** Wowza Streaming Engine <= 4.8.11+5  
**Vulnerability Advisory:** [https://n4nj0.github.io/advisories/wowza-streaming-engine-i/](https://n4nj0.github.io/advisories/wowza-streaming-engine-i/)  

TWowza Streaming Engine (known as Wowza Media Server) is a unified streaming media server software developed by Wowza Media Systems based in Colorado, in the United States of America and used by many US government entities such as NASA, US Air force, Boeing, New York Police Department and many other clients around the world.  
I've found a uncontrolled resource consumption which enables a remote attacker to exhaust filesystem resources via the */enginemanager/server/vhost/historical.jsdata* `vhost` parameter. A successful exploit could allow the attacker to cause database errors and cause the device to become unresponsive to web-based management.

### Usage
`./dos-exploit-wse.py -u http://wse.local:8088 -s CDA32846E8763F62293AAE42FA72C86B`  
`./dos-exploit-wse.py --url http://wse.local:8088 --session CDA32846E8763F62293AAE42FA72C86B`  
