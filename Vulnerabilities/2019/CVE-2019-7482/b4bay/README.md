# Multiple Vulnerabilities in SonicWall SMA/SRA Appliances (CVE-2019-7481, CVE-2019-7482,CVE-2019-7483, CVE-2019-7484, CVE-2019-7485, CVE-2019-7486)
**First Published:** 26 Feb 2020 14:00 GMT  
**Status:** *Developing*  
**Last Updated:** 26 Feb 2020 14:00 GMT  
**CVSS Score:** 9.8  

## Summary

On 18 Dec 2019, SonicWall CSIRT released Security Advisories SNWLID-2019-0016 to SNWLID-2019-0021 [[link](https://psirt.global.sonicwall.com/vuln-list)], which identified a vulnerabilities in SonicWall Secure Mobile Access (SMA) appliances. The most dangerous one among them, [CVE-2019-7482](https://nvd.nist.gov/vuln/detail/CVE-2019-7482), could allow an unauthenticated attacker to execute arbitrary code on the device.

The vulnerability received a score of **9.8** and was deemed **Critical**.

Affected software builds are **9.0.0.3** and earlier. Vulnerabilities have been fixed in the release **9.0.0.4**, which is available for download from [My SonicWall Download Center](https://www.mysonicwall.com/muir/ui/downloadcenter).

The original advisory stated only SMA100 appliance are vulnerable, but my tests showed that at least SMA500v and old Secure Remote Access (SRA) appliances share the same code base and therefore also vulnerable. SMA200 and SMA400 running vulnerable software version also could be at risk. SMA 6200, 7200, 8200v and 9000 seem to be built on other platform and not affected.

## Key vulnerability events

**18 Dec 2019:** SonicWall announces vulnerabilities.  
**11 Feb 2020:** Alain Mowat, who had found the vulnerabilities, published [the details](https://blog.scrt.ch/2020/02/11/sonicwall-sra-and-sma-vulnerabilties/).  
**26 Feb 2020:**  Scope of vulnerability estimated by this report.  

## Vulnerability spread and fix progress

As of 2 Mar 2020 there are about **15,000** relevant devices on the Internet, and about **77%** of them seem to be vulnerable.
About **1.7%** (**269** devices) have been patched since last week.

![CVE-2019-7482-spreading](https://github.com/b4bay/CVE-2019-7482/raw/master/CVE-2019-7482-spreading.png)

TOP-10 of AS owners by count of vulnerable devices is shown below.

| Organization                                              | # of devices |
| --------------------------------------------------------- | :----------: |
| Comcast  Cable Communications, LLC                        |    1,057     |
| MCI  Communications Services, Inc. d/b/a Verizon Business |     403      |
| Charter  Communications Inc                               |     399      |
| AT&T Services, Inc.                                       |     366      |
| Deutsche Telekom AG                                       |     298      |
| Cablevision Systems Corp.                                 |     266      |
| Level 3 Parent, LLC                                       |     238      |
| Asahi Net                                                 |     231      |
| Cox  Communications Inc.                                  |     228      |
| NTT Communications Corporation                            |     218      |

> If you are security representative of AS owner or country CERT who wants to track the spread of the vulnerability in the networks you are responsible please contact me via [email](mailto:b4bay@b4bay.com) or [Twitter](https://twitter.com/b4baysky). All data sets are freely available for authorized persons.

## Exploit Detection

At the moment there is no public exploit available. Based on information disclosed by Alain Mowat, these entry points should be monitored:

- **GET** to **/cgi-bin/supportLogin**, for SQL Injection patterns in *customerTID* parameter,
- **GET** to **/cgi-bin/supportLogin**, for unusually long *User-Agent* header values,
- **GET** to **/cgi-bin/handleWAFRedirect**, for Path Traversal patterns in *hdl* parameter.

Unfortunately, network traffic to the vulnerabilities entry points is encrypted by TLS, so there is no convenient way to detect exploiting attempts. You have to manage to obtain the unencrypted copy of traffic before applying detection rules. 

## In-the-Wild Exploiting

At this point there are no evidence of any public exploitation attempts. 

## Acknowledgements

Thanks to Alain Mowat ([@plopz0r](https://twitter.com/plopz0r)) of SCRT who discovered these vulnerabilities.

## Contacts

Twitter: [@b4baysky](https://twitter.com/b4baysky)  
Email: b4bay@b4bay.com

## Links

https://psirt.global.sonicwall.com/vuln-list  
https://blog.scrt.ch/2020/02/11/sonicwall-sra-and-sma-vulnerabilties/  

