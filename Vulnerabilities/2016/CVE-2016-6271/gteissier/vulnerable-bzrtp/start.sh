#!/bin/bash

WHO=$1

if [ x"$WHO" == x"alice" ]; then
  LOCAL=198.42.12.2
  PEER=198.42.42.2
  GW=198.42.12.254
fi

if [ x"$WHO" == x"bob" ]; then
  LOCAL=198.42.42.2
  PEER=198.42.12.2
  GW=198.42.42.254
fi

ip route add $PEER via $GW dev eth0

sleep 5
echo "          Starting vulnerable BZRTP agent"

/root/agent $PEER
