#Download base image ubuntu 20.04
FROM ubuntu:20.04

# LABEL about the custom image
LABEL maintainer="uzo"
LABEL version="0.1"
LABEL description="This is custom Docker Image for \
salt version 3002."

# Disable Prompt During Packages Installation ( you no loonger need -y)
ARG DEBIAN_FRONTEND=noninteractive

# Update Ubuntu Software repository and install python3, pip and openssh. Cleanup apt cache
RUN apt update && apt-get install -y unzip wget python3 python3-pip openssh-server && pip install cherrypy && \
    rm -rf /var/lib/apt/lists/* && apt clean 

#Copy required resources
COPY resources /root/salt_installer/

WORKDIR /root/salt_installer/

#Create salt directory and copy the installation and configuration files
RUN wget https://github.com/saltstack/salt/archive/refs/tags/v3002.zip && unzip v3002.zip && \
    mkdir -p /etc/salt/ && cp /root/salt_installer/master /etc/salt/master && cp /root/salt_installer/roster /etc/salt/roster && \ 
    #Run the salt install script
    python3 /root/salt_installer/salt-3002/setup.py install 

ENTRYPOINT salt-master -d && salt-api -d && /bin/bash

# Expose Port for the Application 
EXPOSE 8000 4505 4506

