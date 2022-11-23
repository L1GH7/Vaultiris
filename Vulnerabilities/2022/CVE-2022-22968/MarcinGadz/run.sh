#!/bin/bash
set -e
mvn clean package
docker build . -t rce-test
docker run -d -p 8080:8080 -e TAJNE='superhaslo123' rce-test
sleep 5
bash ./exploit.sh