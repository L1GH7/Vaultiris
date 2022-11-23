#!/usr/bin/env bash

docker build . -t cve_2022_23222:latest
docker run --rm -v $(pwd):/data cve_2022_23222:latest /bin/bash -c "pushd /data && cargo build --release"                                                                                                                         ─╯
