# CVE-2021-31703
CVE-2021-31703

Frontier ichris through 5.18 allows users to upload malicious executable files that might later be downloaded and run by any client user.

The application takes any file (e.g. asp, php, exe) and does not verify it's content or it's extension. Any malicious file can be uploaded. 
