#!/bin/bash
set -ex

vmName="MikroTik_Environement"

#ImgVersion="6.41.3"   ## long term version
#ImgVersion="6.45.9"   ## long term version
#ImgVersion="6.46.4"    ## stable release
#ImgVersion="6.47"    ## stable release
#ImgVersion="6.47.1"    ## stable release
#ImgVersion="6.47.2"    ## stable release
ImgVersion="6.47.3"    ## stable release
#ImgVersion="7.0beta5" ## beta
#ImgVersion="7.1beta2" ## beta

## Remove VM From Previous Run
if VBoxManage list vms | grep $vmName; then
  echo "Found $vmName - delete it!"
  VBoxManage controlvm $vmName poweroff || true
  sleep 5
  VBoxManage unregistervm  $vmName --delete
  rm -rf $path/$vmName
  sleep 5
  exit
fi


## Create Install Directory
rm -rf mk_env
mkdir -p mk_env
cd mk_env
path=$(pwd)

## Install Requirements
sudo apt-get update;
sudo apt-get install -y smbclient virtualbox arp-scan

## Download MikroTik RouterOs
wget https://download.mikrotik.com/routeros/$ImgVersion/chr-$ImgVersion.vmdk -O "$path/routerOs.vmdk"


## Start RouterOs
VBoxManage createvm --name $vmName --ostype "Debian_64" --register --basefolder "$path"
VBoxManage modifyvm $vmName --ioapic on
VBoxManage modifyvm $vmName --memory 1024 --vram 128
VBoxManage hostonlyif create
VBoxManage modifyvm $vmName --nic1 hostonly --hostonlyadapter1 vboxnet0 --macaddress1 aaaaaaaaaaaa
VBoxManage storagectl $vmName --name "SATA" --add sata --controller IntelAhci
VBoxManage storageattach $vmName --storagectl "SATA" --port 0 --device 0 --type hdd --medium "$path/routerOs.vmdk"

VBoxManage startvm $vmName

## FIND IP FROM VM
echo "Wait till VM is online"
sleep 60
echo "Scan for VM Ip"
sudo arp-scan --interface=vboxnet0 --localnet | grep "aa:aa:aa:aa:aa:aa"

