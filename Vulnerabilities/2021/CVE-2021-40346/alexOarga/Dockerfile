FROM haproxy:2.2.16-alpine

USER root

COPY haproxy.cfg /usr/local/etc/haproxy/haproxy.cfg
COPY start.sh /usr/src

RUN ["chmod", "+x", "/usr/src/start.sh"]
