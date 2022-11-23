# Libssh2-Exploit

Goals
=====
In this project, we aim to create an exploit to an out of bounds read vulnerabulity in libssh2 described in CVE-2019-13115. Create a malicious ssh server to cause a client connecting to it to crash and see if this can be exploited further to steal data from the client.

Getting the OpenSSH Server Running
==================================
* Download and extract openssh-8.1p1.tar.gz(for unedited OpenSSH server) or openssh-malicious.zip(for the malicious server).
* Go to the folder and run the below commands
  1) ./configure --with-md5-passwords --with-pam --with-selinux --with-privsep-path=/var/lib/sshd/ --sysconfdir=/etc/ssh --with-audit=debug --disable-strip 
  2) make
  3) sudo make install
  4) sudo /usr/local/sbin/sshd

Setting up libssh2 and verifying crash
======================================
* Download and extract libssh2-1.8.2.tar.gz to any folder, say /usr/src/libssh2-1.8.2
* Run the following commands
  1) ./configure
  2) make
  3) make install
  4) ./ssh2 127.0.0.1 \<username\> \<password\>  ***This will trigger a crash***
* to compile any C program having libssh2 libraries, use the following commands
  1) gcc -g -I /usr/src/libssh2-1.8.2/include -I /usr/src/libssh2-1.8.2/src -L /usr/local/lib ssh2.c -o ssh222 -lssh2
  2) If while running, if there are issues with dynamically linked libraries
      <br/>a) ensure the libraries are present in /usr/local/lib
      <br/>b) LD_LIBRARY_PATH=/usr/local/lib
      <br/>c) export LD_LIBRARY_PATH
