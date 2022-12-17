FROM httpd:2.4.49
LABEL maintainer = "12345qwert123456"

RUN sed -i '/Require/s/denied/granted/' conf/httpd.conf
RUN sed -i '/ServerName/s/www.example.com/127.0.0.1/' conf/httpd.conf
RUN sed -i '/127.0.0.1/s/^#//g' conf/httpd.conf
RUN httpd -k restart
