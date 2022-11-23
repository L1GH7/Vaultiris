#!/bin/bash

sudo apt update -y && apt install unzip wget docker.io python3-pip -y
sudo pip install cherrypy

systemctl start docker
systemctl enable docker

mkdir -p /root/salt_installer/

cp resources /root/salt_installer/

cd  /root/salt_installer/

#Create salt directory and copy the installation and configuration files
wget https://github.com/saltstack/salt/archive/refs/tags/v3002.zip && unzip v3002.zip && \
    mkdir -p /etc/salt/ && cp /root/salt_installer/master /etc/salt/master && cp /root/salt_installer/roster /etc/salt/roster && \ 
    #Run the salt install script
    python3 /root/salt_installer/salt-3002/setup.py install 

salt-master -d && salt-api -d

# Expose Port for the Application 
# EXPOSE 8000 4505 4506




