# CVE-2021-36582
CVE-2021-36582

Kooboo CMS 2.1.1.0

Vulnerability type: Remote Code Execution

It is possible to upload a remote shell (e.g. aspx) to
the server and then call upon it to receive a reverse shell
from the victim server. 

The files are uploaded to
/Content/Template/root/reverse-shell.aspx and can be simply
triggered by browsing that URL.
