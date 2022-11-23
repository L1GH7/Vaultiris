FROM alpine:latest

USER root

RUN apk add python3
RUN apk add openjdk8
RUN PATH=$PATH:/usr/lib/jvm/java-1.8-openjdk/bin

COPY ./files/docker-entrypoint.sh /root/docker-entrypoint.sh
COPY ./files/Exploit.java /

RUN /usr/lib/jvm/java-1.8-openjdk/bin/javac /Exploit.java

ENTRYPOINT /root/docker-entrypoint.sh
