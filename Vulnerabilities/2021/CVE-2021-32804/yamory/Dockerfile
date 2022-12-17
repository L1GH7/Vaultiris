FROM node:16.7.0-buster

RUN apt update && apt install -y vim \
&& npm i -g n
COPY ./poc /poc
WORKDIR /poc
