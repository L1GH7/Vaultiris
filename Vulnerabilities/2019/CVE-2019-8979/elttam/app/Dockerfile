FROM php:7.4.3-apache
RUN docker-php-ext-install mysqli pdo pdo_mysql
COPY apache-vhost.conf /etc/apache2/sites-enabled/app.conf
RUN a2enmod rewrite
RUN curl -s https://codeload.github.com/koseven/koseven/tar.gz/3.3.9 -o - | tar xzf - && mv koseven-3.3.9 /app
WORKDIR /app
COPY src /app
RUN sed -i \
  -e "s#// 'database'#'database'#" -e "s#// 'orm'#'orm'#" \
  -e 's#// Cookie::$salt = NULL#Cookie::$salt = "weeeeee"#'  application/bootstrap.php
RUN sed -i \
  -e "s#'hostname'   => 'localhost'#'hostname'   => 'db'#" \
  -e "s#'username'   => FALSE#'username'   => 'root'#" \
  -e "s#'password'   => FALSE#'password'   => 'root'#" modules/database/config/database.php
RUN chown www-data:www-data application/cache application/logs && rm public/install.php
