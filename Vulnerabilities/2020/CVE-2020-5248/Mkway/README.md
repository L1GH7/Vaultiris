# CVE-2020-5248 POC 환경 구성 및 테스트 입니다. 

# 테스트 방법

- 환경 구성 

```
vim docker/nginx/default.conf
server_name [docker host ip ];  -change
mysqlserver: mysql  id: root pw: root 
```
- 실행 

```
cd docker/app/
tar -xvf /docker/app/glpi-9.4.5.tgz
chmod -R 777 glpi
docker-compose up -d 
```

glpi 접속 http://localhost:8080/glpi

- POC 테스트  
poc : https://github.com/indevi0us/CVE-2020-5248    
테스트 : https://offsec.almond.consulting/multiple-vulnerabilities-in-glpi.html     
암호문 획득 - http://localhost:8080/glpi 
```
docker/poc/decrypt_any.php
vim decrpyt_any.php - change - decrypt ("decrytion")
http://localhost:8080/poc/decrypt_any.php 
```

---- 
# 주의 
설치 실패시 한번 glpi를 삭제후 다시 tar 작업을 진행하십시오
