FROM alpine:latest


ARG server_ip=172.17.0.2
ADD poc.tar.gz poc
ENV SERVER=$server_ip
ENTRYPOINT ["tail", "-f", "/dev/null"]

RUN apk update \
&& apk add tar \
&& apk add netcat-openbsd \
&& apk add sed

WORKDIR /poc
RUN sed -i 's/127.0.0.1/'"$SERVER"'/' crash.sh
RUN sed -i 's/8000/80/' crash.sh
RUN chmod 777 crash.sh

EXPOSE 8080 8080
# Use This Command to Run Attack automatically. Comment this out and connect to the Container to Run ist manually.
#CMD ./crash.sh

