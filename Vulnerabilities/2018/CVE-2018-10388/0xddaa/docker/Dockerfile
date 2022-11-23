FROM ubuntu:18.04
MAINTAINER ddaa
RUN apt update
RUN apt install xinetd -y
RUN apt install libc6-dev-i386 -y
RUN apt install python3 -y
RUN useradd -m opentftp
RUN chmod 774 /tmp
RUN chmod -R 774 /var/tmp
RUN chmod -R 774 /dev
RUN chmod -R 774 /run
RUN chmod 1733 /tmp /var/tmp /dev/shm
RUN chown -R root:root /home/opentftp
CMD ["/usr/sbin/xinetd", "-dontfork"]
