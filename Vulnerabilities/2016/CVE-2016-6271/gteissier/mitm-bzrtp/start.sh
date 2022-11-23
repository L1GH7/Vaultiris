#!/bin/bash

iptables -A FORWARD -d 198.42.42.2 -p UDP --dport 4919 -j DROP
iptables -A FORWARD -d 198.42.12.2 -p UDP --dport 4919 -j DROP

/root/attack.py
