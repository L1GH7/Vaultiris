#!/bin/bash
docker build -t sort_code_exec .
docker run --name=sort_code_exec --rm -p1337:80 -it sort_code_exec
