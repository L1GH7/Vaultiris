# CVE-2021-36581
CVE-2021-36581

Vulnerability type: Remote - file upload

Insecure file upload in Kooboo CMS 2.1.1.0
It is possible to upload any file extension to the
server. The server does not verify the extension of the file
and the tester was able to upload an aspx to the server.

File upload directory:
/Content/Kooboo_BinaryResource/UploadFile

