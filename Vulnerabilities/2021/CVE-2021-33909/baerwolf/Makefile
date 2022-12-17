ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := cve-2021-33909.o
cve-2021-33909-y := cve-2021-33909_core.o

else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

endif
