#!/usr/bin/env bash

LD_PRELOAD=$PWD/interpose.so libvirtd &
LD_LIBRARY_PATH=server/install/lib ./perftest $1
pkill -9 libvirtd
