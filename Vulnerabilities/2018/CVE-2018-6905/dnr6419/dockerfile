From ubuntu:bionic 
ENV LC_ALL=C.UTF-8
ENV TZ=Asia/Seoul
ENV TYPO3_VERSION=8.7.8

RUN DEBIAN_FRONTEND=noninteractive
RUN sed -ie 's/archive.ubuntu.com/mirror.kakao.com/g' /etc/apt/sources.list
RUN apt-get update && apt-get upgrade -y 
RUN apt-get install -y debconf-utils
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN apt-get install -y -q apt-utils git wget ssh apache2 php dialog \  
    php7.2-gd php7.2-mysqli php7.2-xml php7.2-zip net-tools iputils-ping 

# {$TYPO3_VERSION}
COPY ./typo3_src-8.7.8.tar.gz /var/www/html/typo3_src-$TYPO3_VERSION.tar.gz 
# ADD is directory

WORKDIR /var/www/html 
RUN chmod -R 777 /var/www/html
RUN chmod -R 777 typo3_src-$TYPO3_VERSION.tar.gz
RUN mkdir /var/run/sshd
RUN echo 'root:123456' | chpasswd
RUN sed -i 's/#*PermitRootLogin prohibit-password/PermitRootLogin yes/g' /etc/ssh/sshd_config

CMD [ "/usr/sbin/sshd","-D" ]

USER www-data
RUN tar -xvf typo3_src-$TYPO3_VERSION.tar.gz 
RUN mv typo3_src-$TYPO3_VERSION typo3
RUN touch /var/www/html/typo3/FIRST_INSTALL

USER root
EXPOSE 80
ENTRYPOINT ["/usr/sbin/apache2", "-k", "start"]
