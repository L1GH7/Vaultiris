#!/bin/bash

if ! type docker-compose &>/dev/null; then
  echo 'docker-compose is required'
  exit 1
fi

docker-compose up -d --build

echo "waiting for mysql to start up"
while :; do
  docker-compose logs db | grep -q 'port: 3306  MySQL Community Server' && break
done

echo "db is up"
docker-compose run --no-deps --rm exploit -b --current-db --current-user --users --dbs
