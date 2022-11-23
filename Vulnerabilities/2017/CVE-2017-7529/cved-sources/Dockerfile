FROM nginx:1.13.1

LABEL author="Medicean (Medici.Yan@Gmail.com)"
LABEL maintainer="cved (cved@protonmail.com)"

ENV LANG C.UTF-8
ENV LANGUAGE C.UTF-8
ENV LC_ALL C.UTF-8

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get -y update \
    && apt-get -y install \
    apache2 \
    && apt-get autoremove \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

COPY build/nginx.conf /etc/nginx/
COPY build/default.conf /etc/nginx/conf.d/
COPY build/index.html /usr/share/nginx/html/
COPY build/000-default.conf /etc/apache2/sites-enabled/
COPY build/ports.conf /etc/apache2/
COPY build/test.png /var/www/html/
COPY build/main.sh /

EXPOSE 80

ENTRYPOINT ["/main.sh"]

CMD ["default"]
