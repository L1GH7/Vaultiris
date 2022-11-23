FROM cved/base-wordpress

LABEL author="cved (cved@protonmail.com)"
LABEL maintainer="cved (cved@protonmail.com)"

ENV LANG C.UTF-8
ENV LANGUAGE C.UTF-8
ENV LC_ALL C.UTF-8

COPY build/wp-gdpr-compliance.1.4.2.zip /tmp/
RUN unzip -x /tmp/wp-gdpr-compliance.1.4.2.zip -d /var/www/html/wp-content/plugins/ \
    && chown -R www-data:www-data /var/www/html/ \
    && rm /tmp/*

COPY build/main.sh /

EXPOSE 80

CMD ["/main.sh"]
