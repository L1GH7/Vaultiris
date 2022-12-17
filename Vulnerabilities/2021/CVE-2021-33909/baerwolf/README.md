Kernel runtime fix of CVE-2021-33909
====================================

This repository builds an out-of-tree module named "cve-2021-33909.ko".
cve-2021-33909 fixes an issue in the kernels filesystem layer by kprobe-replacing vulnerable functions during runtime.

It can be build via "make", setting KDIR to the kernels (source-/header-)directory.

It can be loaded directly via "insmod cve-2021-33909.ko",
or copied (+depmod) into the kernels module directory (extra/) and then modprobed with "modprobe cve-2021-33909".

by Stephan Baerwolf, Erfurt 2021
