from php:7.2-apache

RUN apt-get update -yyq && apt-get install -yyq zip zlib1g-dev libzip-dev libxml2-dev curl libcurl4-openssl-dev libpng-dev libjpeg62-turbo-dev
RUN docker-php-ext-configure gd --with-jpeg-dir=/usr/include/
RUN docker-php-ext-install zip json xml curl gd exif mysqli
RUN a2enmod rewrite

ADD ossn-5.2.tar.xz /var/www/html/
RUN mkdir -p /var/www/ossn_data/

RUN chown -R www-data:www-data /var/www/
RUN chmod -R 755 /var/www/
RUN rm -rf /var/log/apache2/access.log /var/log/apache2/error.log
ADD 000-default.conf /etc/apache2/sites-available/000-default.conf

# The site will be availiable on http://