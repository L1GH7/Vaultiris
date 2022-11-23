#!/usr/bin/env bash

docker build . -t demonstration
docker run -t -i --rm demonstration
