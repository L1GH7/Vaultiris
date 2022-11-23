FROM debian
MAINTAINER gteissier@gmx.com

RUN apt-get update && apt-get install -y tcpdump \
  iptables \
  python3 python3-crypto python3-pip \
  nettle-dev libgmp-dev \
&& pip3 install scapy-python3 \
&& rm -rf /var/lib/apt/lists/*

ADD start.sh /root/start.sh
ADD attack.py /root/attack.py
COPY zrtp /root/zrtp

ADD Makefile /root/Makefile
ADD bruteforce.c /root/bruteforce.c

RUN cd /root && make
