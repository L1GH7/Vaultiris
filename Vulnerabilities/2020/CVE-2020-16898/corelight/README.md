# "Bad Neighbor" Detection, CVE-2020-16898 (Windows TCP/IP RCE) 

## Summary:  
A network detection package for CVE-2020-16898 (Windows TCP/IP Remote Code Execution Vulnerability)

## References: 
- https://corelight.blog/2020/10/15/zeek-community-activates-to-detect-bad-neighbor-cve-2020-16898/
- https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/CVE-2020-16898#ID0EUGAC
- Other detection packages developed independently and concurrently by the Zeek community:
https://github.com/initconf/CVE-2020-16898-Bad-Neighbor/blob/master/scripts/CVE-2020-16898-Bad-Neighbor.zeek  
https://github.com/esnet-security/cve-2020-16898

## Notices raised :   

```CVE-2020-16898 exploit detected from %s. https://portal.msrc.microsoft.com/en-us/security-guidance/advisory/CVE-2020-16898#ID0EUGAC . Details from packet for reference: info=%s , options=%s```


## Usage, notes and recommendations:
- To use against a pcap you already have ```zeek -Cr scripts/__load__.zeek your.pcap```   
- This package will run in clustered or non clustered environments.  

## Feedback
- As details emerge, we are keen to improve this package for the benefit of the community, please feel free to contact the author with any suggestions and feedback.
