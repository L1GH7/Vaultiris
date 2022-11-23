FROM archlinux:latest
#RUN opkg update && opkg install uhttpd#
#COPY ./uhttpd-5f9ae57.tar.gz ./uhttpd-5f9ae57.tar.gz
ADD uhttpd-5f9ae57.tar.gz  uhttpd
ADD libubox.tar.gz  libubox
ADD uhttpbuild.tar.gz server
#ENTRYPOINT ["tail", "-f", "/dev/null"]
#RUN apt-get update && apt install -y libjson-c5 && apt-get install -y libssl-dev && apt install -y libcrypt-dev && apt install -y lua5.1 &&apt-get install -y cmake && apt-get install -y make
RUN pacman -Syu --noconfirm json-c
RUN pacman -Sy --noconfirm libxcrypt
RUN pacman -Sy --noconfirm lua51 
RUN pacman -Sy --noconfirm cmake 
RUN pacman -Sy --noconfirm make
#RUN apt-get update \
#&& apk add tar \
#&& apk add cmake\
# && apt-get install make
# && apt-get install build-base\
#&& apt-get install json-c-dev \
#&& apk add nano\ 
#&& apk add sed \
# && apt-get install lua5.4 5.4.4-1
#&& apk add lua-dev\
#&& apk add tree \
#&& apk add libc6-compat\
#&& apk add libcrypt.so.2
# && apk add ubox
#RUN ls -lah
# WORKDIR uhttpd/uhttpd-5f9ae57/
# RUN mkdir libubox
# RUN mv /libubox/libubox-f2b2ee4/*  libubox
#RUN cd libubox && cmake . && make 
# RUN sed -i '2i cmake_policy(SET CMP0021 OLD)'  CMakeLists.txt
# RUN sed -i '25,26s/.*//' CMakeLists.txt
# RUN sed -i '25 s?.*?INCLUDE_DIRECTORIES('`pwd`'/libubox/)?' CMakeLists.txt
#RUN cmake .
#RUN make
COPY uhttpd.conf /etc/uhttpd.conf
COPY index.html /www/index.html
WORKDIR /server
RUN mkdir -p cgi-bin; touch cgi-bin/crash; chmod +x cgi-bin/crash


EXPOSE 80 80
RUN chmod 777 uhttpd
CMD ./uhttpd -f -p 0.0.0.0:80
