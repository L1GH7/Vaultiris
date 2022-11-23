FROM maven:alpine

RUN apk update && apk add git
RUN git clone https://github.com/trustathsh/irond.git
RUN cd irond && mvn package

RUN cp -r /irond/src/main/templates/* /irond/target/
RUN mkdir -p /etc/contrail/
COPY openstackrc /etc/contrail/

WORKDIR /irond/target
EXPOSE 8443 8444

CMD java -jar irond-0.5.8-bin.jar
