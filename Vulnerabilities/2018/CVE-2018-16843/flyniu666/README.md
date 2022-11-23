BUILD:
```bash
cd ./ingress-nginx-0.21-1.19.5
cd images/nginx/
make
cd  -
cd images/e2e/
make docker-build
cd -
e2e_tag=`docker images|grep e2e|awk {'print  $2'}`
sed -i "s/e2e:v12232020-dce860a/e2e:$e2e_tag/" build/go-in-docker.sh
make


echo -e '\n\n\n\n\n'
docker images|grep  ingress-nginx
```

