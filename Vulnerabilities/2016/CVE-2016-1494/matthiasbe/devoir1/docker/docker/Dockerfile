FROM library/ubuntu:latest

RUN apt-get update

RUN apt-get --assume-yes install python3
RUN apt-get --assume-yes install python3-pip
RUN apt-get --assume-yes install python3-gmpy2

COPY src /src
COPY include /include

