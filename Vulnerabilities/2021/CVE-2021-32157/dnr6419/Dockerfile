FROM ubuntu:bionic
MAINTAINER "0ppr2s" <dnr6419@gmail.com>

RUN sed -i 's@archive.ubuntu.com@mirror.kakao.com@g' /etc/apt/sources.list
# base installation
RUN apt-get update && apt-get install -y vim wget curl \
		perl libnet-ssleay-perl \
		libauthen-pam-perl \
		libpam-runtime libio-pty-perl \
		shared-mime-info \
		python unzip \
		cron 
# download the webmin 1.973
# COPY ./webmin_1.973_all.deb webmin_1.973_all.deb
RUN wget https://jaist.dl.sourceforge.net/project/webadmin/webmin/1.973/webmin_1.973_all.deb
# installation
RUN dpkg -i webmin_1.973_all.deb

EXPOSE 10000
ENV LC_ALL C.UTF-8

# set the id and password 
RUN echo root:123456 | chpasswd
# ssl disable
RUN sed -i 's/ssl=1/ssl=0/g' /etc/webmin/miniserv.conf
WORKDIR /home

CMD /usr/bin/touch /var/webmin/miniserv.log && /usr/sbin/service webmin restart && /usr/bin/tail -f /var/webmin/miniserv.log
