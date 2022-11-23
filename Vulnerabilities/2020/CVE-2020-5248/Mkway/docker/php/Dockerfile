FROM php:7.4.21-fpm-alpine
RUN apk update --update-cache \
    && apk add --no-cache mysql-dev 

COPY --from=mlocati/php-extension-installer /usr/bin/install-php-extensions /usr/local/bin/

RUN install-php-extensions gd xdebug \ 
			&& install-php-extensions ldap \
			&& install-php-extensions imap \
			&& install-php-extensions mysqli
