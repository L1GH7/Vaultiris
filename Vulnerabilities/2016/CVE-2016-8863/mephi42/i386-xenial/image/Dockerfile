FROM i386/ubuntu:xenial as i386-xenial
SHELL ["/bin/bash", "-o", "pipefail", "-c"]
RUN sed -i -e 's/^# deb-src /deb-src /g' /etc/apt/sources.list
RUN dpkg --add-architecture amd64
RUN apt-get -y update && \
    apt-get -y install --no-install-recommends \
        ca-certificates \
        gzip \
        libc6:amd64 \
        libc6-dbg \
        tar \
        wget && \
    apt-get -y build-dep --no-install-recommends \
        libupnp && \
    (cd /usr/src && apt-get source libc6) && \
    apt-get clean && \
    rm -r /var/lib/apt/lists/*
WORKDIR /opt
RUN wget -q -O- https://github.com/pupnp/pupnp/tarball/release-1.6.18 | tar -xz && \
    mv pupnp-pupnp-6cd1f11 pupnp
WORKDIR /opt/pupnp
RUN ./bootstrap
RUN ./configure
RUN make -j"$(getconf _NPROCESSORS_ONLN)" CFLAGS='-pthread -g -O0 -Wall'

FROM ubuntu:focal
SHELL ["/bin/bash", "-o", "pipefail", "-c"]
RUN apt-get -y update && \
    apt-get -y install --no-install-recommends \
        gdb \
        gdbserver \
        libffi-dev \
        libssl-dev \
        python3 \
        python3-dev \
        python3-pip \
        python3-setuptools \
        wget \
        xterm && \
    apt-get clean && \
    rm -r /var/lib/apt/lists/*
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install --upgrade https://github.com/mephi42/pwntools/zipball/executable-relative-to-cwd
RUN wget -q -O- https://github.com/hugsy/gef/raw/master/scripts/gef.sh | sh
COPY --from=i386-xenial / /i386-xenial/
RUN mkdir -p /usr/lib/debug && \
    ln -s . /i386-xenial/usr/lib/debug/i386-xenial
