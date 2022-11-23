#!/bin/bash

#
# build root fs
#
pushd fs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
popd

#
# launch
#
/usr/bin/qemu-system-aarch64 \
	-machine virt \
	-cpu cortex-a57 -machine type=virt \
	-kernel linux-4.14.186/arch/arm64/boot/Image \
	-initrd $PWD/initramfs.cpio.gz \
	-fsdev local,security_model=passthrough,id=fsdev0,path=$PWD \
	-device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare \
	-nographic \
	-monitor none \
	-smp cores=2,threads=1,sockets=1 \
	-s \
	-m 4096 \
	-append "console=ttyAMA0 nokaslr"
