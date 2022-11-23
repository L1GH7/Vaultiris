FROM alpine:3.13

# Install system packages
RUN apk add --no-cache --update supervisor hitch php7 zip curl apache2 \
    php7-apache2 php7-json php7-phar php7-iconv php7-mbstring php7-xml \
    openssl php7-openssl php7-ctype php7-dom php7-session php7-curl php7-zip

# Fix iconv library
ENV LD_PRELOAD /usr/lib/preloadable_libiconv.so php

# Download composer
RUN wget https://getcomposer.org/composer-1.phar -O /usr/local/bin/composer && chmod +x /usr/local/bin/composer

# Copy configuration files
COPY config/supervisord.conf /etc/supervisord.conf
COPY config/httpd.conf /etc/apache2/httpd.conf

# Setup permissions
COPY --chown=root example /www

# Switch working environment
WORKDIR /www


RUN php7 /usr/local/bin/composer install

EXPOSE 443

# Run supervisord
CMD /usr/bin/supervisord -c /etc/supervisord.conf
