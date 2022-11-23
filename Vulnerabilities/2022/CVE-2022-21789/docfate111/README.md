# testing_Android_driver
bug i reported but someone reported first 


Scripts based on [pwnkernel](https://github.com/pwncollege/pwnkernel)
Building the kernel, busybox, and demo modules:

```
$ ./build.sh
```

Running the kernel:

```
$ ./launch.sh
```

All modules will be in `/`, ready to be `insmod`ed, and the host's home directory will be mounted as `/home/ctf` in the guest.
