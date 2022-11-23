FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y install acl apt-utils attr autoconf bind9utils binutils bison build-essential ccache chrpath curl debhelper dnsutils docbook-xml docbook-xsl flex gcc gdb git glusterfs-common gzip heimdal-multidev hostname htop krb5-config krb5-kdc krb5-user language-pack-en lcov libacl1-dev libarchive-dev libattr1-dev libavahi-common-dev libblkid-dev libbsd-dev libcap-dev libcephfs-dev libcups2-dev libdbus-1-dev libglib2.0-dev libgnutls28-dev libgpgme11-dev libicu-dev libjansson-dev libjs-jquery libjson-perl libkrb5-dev libldap2-dev liblmdb-dev libncurses5-dev libpam0g-dev libparse-yapp-perl libpcap-dev libpopt-dev libreadline-dev libsystemd-dev libtasn1-bin libtasn1-dev libtracker-sparql-2.0-dev libunwind-dev lmdb-utils locales lsb-release make mawk mingw-w64 patch perl perl-modules pkg-config procps psmisc python3 python3-cryptography python3-dbg python3-dev python3-dnspython python3-gpg python3-iso8601 python3-markdown python3-matplotlib python3-pexpect python3-pyasn1 python3-setproctitle rng-tools rsync sed sudo tar tree uuid-dev wget xfslibs-dev xsltproc zlib1g-dev python-dev net-tools tcpdump nano
WORKDIR /tmp
RUN wget https://download.samba.org/pub/samba/stable/samba-4.5.16.tar.gz
RUN tar xf samba-4.5.16.tar.gz
WORKDIR /tmp/samba-4.5.16
RUN ./configure --prefix=/usr --enable-fhs --sysconfdir=/etc --localstatedir=/var --with-privatedir=/var/lib/samba/private --with-smbpasswd-file=/etc/samba/smbpasswd --with-piddir=/var/run/samba --with-pammodulesdir=/lib/x86_64-linux-gnu/security --libdir=/usr/lib/x86_64-linux-gnu --with-modulesdir=/usr/lib/x86_64-linux-gnu/samba --datadir=/usr/share --with-lockdir=/var/run/samba --with-statedir=/var/lib/samba --with-cachedir=/var/cache/samba --with-socketpath=/var/run/ctdb/ctdbd.socket --with-logdir=/var/log/ctdb --without-ad-dc --enable-debug && make -j 4 && make install -j 4
RUN mkdir /TimeMachineBackup
RUN chown nobody:nogroup /TimeMachineBackup
RUN chmod 2770 /TimeMachineBackup
RUN echo "[global]\nmap to guest = Bad User\nvfs objects = acl_xattr\nmap acl inherit = yes\n[TimeMachineBackup]\npath = /TimeMachineBackup\nguest ok = yes\nfruit:time machine = yes\nfruit:time machine max size = 0M\nvfs objects = catia fruit streams_xattr\nguest ok = yes\nwritable = yes\nea support = yes" > /etc/samba/smb.conf
