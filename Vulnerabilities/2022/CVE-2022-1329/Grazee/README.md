# CVE-2022-1329-WordPress-Elementor-RCE

This poc is based on the one of [AkuCyberSec](https://github.com/AkuCyberSec/CVE-2022-1329-WordPress-Elementor-3.6.0-3.6.1-3.6.2-Remote-Code-Execution-Exploit).

[中文版本](./README-zh.md)

# Usage

Use `docker-compose` to start the vuln environment:

```shell
docker-compose up -d
```

Then you can access the WordPress Admin page with URL `http://localhost:8000`.

Register the admin user, and add a **no-admin** user in the **Users** tag. For example, there are two users:

| Username | Password | Role   |
|----------|----------|--------|
| wc       | 123456   | Admin  |
| a2       | aaaaaa   | Editor |

Log in with the admin account, and install **Elementor 3.6.1** with the zip provided.

If you encounter the upload size problem, add **php.ini** to `/var/www/html/` as following and restart the docker server:

```text
upload_max_filesize = 64M
max_execution_time = 0
post_max_size = 150M
file_uploads = on
```

Edit the poc script with your **no-admin** user infomation and run the poc script to exploit:

```shell
python3 poc.py
```

**NOTE:** the script may failed with **upload problem**, but it's **OK**, try to refresh the admin page in the browser to see if it works.

The poc now runs `phpinfo();` as it's payload, of course you can edit this, but you need to manual remove the `elementor-pro` plugin in docker container `/var/www/html/wp-content/plugin/elementor-pro`:

```shell
# Unzip the zip file
unzip elementor-pro.zip

# Edit the main file to put your payload, it's in the begin of the file
nano elementor-pro/elementor-pro.php

# And zip
rm elementor-pro.zip; zip -ry elementor-pro.zip elementor-pro

# exploit again
python3 poc.py
```

![Result](1.png)
