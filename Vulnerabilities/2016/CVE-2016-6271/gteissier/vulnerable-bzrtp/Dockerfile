FROM debian
MAINTAINER gteissier@gmx.com

RUN apt-get update && apt-get install -y \
  git \
  automake \
  libtool \
  libpolarssl-dev \
  pkg-config \
  build-essential \
  cmake \
  iproute2 \
  tcpdump \
&& rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/BelledonneCommunications/bctoolbox.git
RUN cd bctoolbox \
&& git reset --hard 901292f519f7267e1ad8aba8f8693c371fc8358b \
&& cmake . -DENABLE_MBEDTLS=NO \
&& make && make install \
&& rm -rf ../bctoolbox

RUN git clone https://github.com/BelledonneCommunications/bzrtp.git
RUN cd bzrtp \
&& git reset --hard 43eef42cac9d4fc6e99f52c06c9202fc81887e88 \
&& ./autogen.sh && ./configure --disable-tests \
&& make && make install \
&& rm -rf ../bzrtp

RUN ldconfig

COPY agent.c /root/
COPY Makefile /root

RUN cd /root && make

COPY start.sh /root/
