#!/bin/bash
#Upgrades OpenSSL to version 1.1.1.q
wget https://www.openssl.org/source/openssl-1.1.1q.tar.gz
sudo tar -xf openssl-1.1.1q.tar.gz
cd openssl-1.1.1q
sudo ./config
sudo make
sudo make install
sudo ldconfig
cd ~
sudo rm openssl-1.1.1q.tar.gz
sudo rm -rf openssl-1.1.1q
sudo systemctl restart nginx
nginx -V
