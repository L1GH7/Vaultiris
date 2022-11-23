FROM library/ubuntu:latest


RUN apt-get update

RUN apt-get --assume-yes install python3 python3-pip

# Installing python modules
RUN pip3 install gmpy2

COPY src /src

