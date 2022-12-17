# Exploit for Apache2

Exploit for path transversal vulnerability in apache.

Version: 2.4.49
CVE: CVE-2021-41773

# Pull docker image
```
docker pull httpd:2.4.49-alpine
```

# Configure apache

In */usr/local/apache2/conf/httpd.conf* replace these entries
```
 <Directory />
     AllowOverride none
-    Require all denied
+    #Require all denied
 </Directory>
```

```
<IfModule mpm_prefork_module>
-       #LoadModule cgi_module modules/mod_cgi.so
+       LoadModule cgi_module modules/mod_cgi.so
</IfModule>
```

# Install dependencies 

```
Bundle install
```

# Execute exploit

```
ruby expoit.rb -t http://localhost 
```