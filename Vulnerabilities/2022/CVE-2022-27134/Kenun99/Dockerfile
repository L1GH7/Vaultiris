FROM ubuntu:18.04

RUN apt update && \
    apt install -y curl wget python vim && \
    wget https://github.com/eosio/eos/releases/download/v2.1.0/eosio_2.1.0-1-ubuntu-18.04_amd64.deb && \
    apt install -y ./eosio_2.1.0-1-ubuntu-18.04_amd64.deb && \
    rm -rf /*.deb /var/lib/apt/lists/*

COPY . /root

WORKDIR /root

RUN chmod +x /root/poc.py

ENTRYPOINT ["/root/poc.py"]
