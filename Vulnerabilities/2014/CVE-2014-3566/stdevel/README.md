poodle_protector
================

``poodle_protector.py`` is a Python script for automatically protecting your systems against POODLE vulnerability ([*CVE-2014-3566*](http://web.nvd.nist.gov/view/vuln/detail?vulnId=CVE-2014-3566)).
It scans your Apache server configuration directories for unsecure configuration directives and ensures security by disabling SSL 2.0 and 3.0. It can also restart your server instantly after customizing the configuration.

Currently this only works for Linux servers running Apache, the support for other affected daemons (*Tomcat, vsftpd, Dovecot, Postfix, openLDAP, CUPS*) is planned (*see __nightly__ branch*). The following Linux distros are detected automatically:
- CentOS / Red Hat Enterprise Linux
- openSUSE / SUSE Linux Enterprise Server
- Debian / Ubuntu
- Fedora



Parameters
==========
```
# ./poodle_protector.py -h
Usage: poodle_protector.py [options]

poodle_protector.py is used to protect your servers against POODLE
vulnerability (CVE-2014-3566). It automatically detects apache configuration
files vulnerable to POODLE and customizes them after creating backups.
Checkout the GitHub page for updates:
https://github.com/stdevel/poodle_protector

Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  -c STRING, --custom-string=STRING
                        defines a custom SSLProtocol configuration string
  -p PATH, --custom-path=PATH
                        defines a custom path for apache configuration files
                        in case you're not using distribution defaults
  -q, --quiet           don't print status messages to stdout
  -d, --debug           enable debugging outputs
  -n, --no-backup       don't create backups if you like to like dangerously
  -l, --dry-run         only simulates what would be done
  -r, --service-restart restarts the affected service(s) using the appropriate
                        wrapper
```



Examples
========
Just do a dry-run to see what the script would do:
```
# ./poodle_protector.py -lr
I'd like to create a backup of '/etc/apache2/mods-available/ssl.conf as '/etc/apache2/mods-available/ssl.conf.20141016-1303' ...
I'd like to insert 'SSLProtocol All -SSLv2 -SSLv3' into /etc/apache2/mods-available/ssl.conf using the following command: sed -i '/SSLProtocol/ c\SSLProtocol All -SSLv2 -SSLv3' /etc/apache2/mods-available/ssl.conf ...
I'd also like to restart the service using: 'service apache2 restart'
```

Ensure security by customizing the configuration and restart the service:
```
# ./poodle_protector.py -r
Restarting web server: apache2 ... waiting .
```

Simulate setting a special ``SSLProtocol`` value (*only TLSv1.2 instead of everything except SSLv2/v3*) and reloading the daemon on a Enterprise Linux server:
```
# ./poodle_protector.py -lrc "SSLProtocol TLSv1.2"
I'd like to create a backup of '/etc/httpd/conf.d/ssl.conf as '/etc/httpd/conf.d/ssl.conf.20141017-1112' ...
I'd like to insert 'SSLProtocol TLSv1.2' into /etc/httpd/conf.d/ssl.conf using the following command: sed -i '/SSLProtocol/ c\SSLProtocol TLSv1.2' /etc/httpd/conf.d/ssl.conf ...
I'd also like to restart the service using: 'service httpd restart'
```
