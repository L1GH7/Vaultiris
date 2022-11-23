# CVE-2020-9461

```
██╗  ██╗███████╗███████╗
╚██╗██╔╝██╔════╝██╔════╝
 ╚███╔╝ ███████╗███████╗
 ██╔██╗ ╚════██║╚════██║
██╔╝ ██╗███████║███████║
╚═╝  ╚═╝╚══════╝╚══════╝
``` 

**Stored XSS - Oempro**

<p>Octech Oempro 4.7 through 4.11 allow stored XSS by an authenticated user. The FolderName parameter of the Media.CreateFolder command is vulnerable.</p>

><p><b>Command:</b>Media.CreateFolder</p>
><p><b>Request parameter:</b>FolderName</p>
><p><b>Version:</b> Oempro v4.7 <= v4.11</p>
><p><b>Researcher:</b> Guilherme Rubert
><p><b>Payload</b> </p>
```
<marquee/onstart=alert("XSS")>
````

<br><br>**References**
<p>https://guilhermerubert.com/blog/cve-2020-9461/</p>
<p>https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-9461</p>
<p>https://nvd.nist.gov/vuln/detail/CVE-2020-9461</p>
<p>https://www.octeth.com/</p>
