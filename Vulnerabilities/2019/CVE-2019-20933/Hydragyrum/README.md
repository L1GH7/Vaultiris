# CVE-2019-20933

> InfluxDB before 1.7.6 has an authentication bypass vulnerability in the authenticate function in services/httpd/handler.go because a JWT token may have an empty SharedSecret (aka shared secret).

(see [https://nvd.nist.gov/vuln/detail/CVE-2019-20933](https://nvd.nist.gov/vuln/detail/CVE-2019-20933) For more details)

## PoC

This PoC exploits the above CVE to make a quick and dirty influxDB client.

## Usage:

usage: influx-client.py [-h] [--host HOST] [--port PORT] [--user USER] [--db DB] query

A simple, silly, over-the-top influxdb client made in Python

positional arguments:
  query                 The query to execute. default: SHOW DATABASES

optional arguments:
  -h, --help            show this help message and exit
  --host HOST           The target IP. (default: localhost)
  --port PORT, -p PORT  The target port. (default: 8086)
  --user USER           The target username. (default: admin)
  --db DB               The database to use.

## Acknowledgements

Portions of code borrowed from https://github.com/LorenzoTullini/InfluxDB-Exploit-CVE-2019-20933
