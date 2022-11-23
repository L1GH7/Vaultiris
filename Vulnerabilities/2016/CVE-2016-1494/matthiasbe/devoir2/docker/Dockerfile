FROM linode/lamp
RUN apt-get update
RUN apt-get -y install php5-mysql
RUN rm /var/www/example.com/public_html/index.html
COPY wordpress /var/www/example.com/public_html

EXPOSE 8080

ADD db.sql /tmp/db.sql
RUN service mysql start && \
  sleep 5 && \
  mysql -u root -pAdmin2015 -e "CREATE DATABASE wp" && \
  mysql -u root -pAdmin2015 wp < /tmp/db.sql

CMD service mysql start ; \
  service apache2 start ; \
  /bin/bash

