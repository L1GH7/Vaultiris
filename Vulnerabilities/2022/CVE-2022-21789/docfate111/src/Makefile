# add more modules here!
obj-m = audio_ipi_driver.o
KERNEL_VERSION=4.14.186
CC=aarch64-linux-gnu-gcc
all: 
	$(CC) -static test.c -o test -lpthread && ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- make -C ../linux-$(KERNEL_VERSION) M=$(PWD) modules && mv *.ko ../fs && mv test ../fs
clean:
	make -C ../linux-$(KERNEL_VERSION) M=$(PWD) clean
