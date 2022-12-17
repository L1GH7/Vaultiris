# CVE-2021-3625

This repository contains a few example exploits for CVE-2021-3625.

All Zephyr-based usb devices up to (and including) version 2.5.0 suffer from
a buffer overflow allowing readout of up to 65kB. Depending on the actual device
this may result of leakage of sensitive data like encryption keys or credentials.

The issue may be triggered by issuing crafted usb control transfer requests
with IN direction (reads, to host) and a write specific request - in example
set address (0x05). In such cases write to host is not expected thus response
length is not updated resulting in a initially set value of request->wLength.
Since wLength is user controlled it can be set to an arbitrary two byte value like
0xffff resulting in readout of 65 kilobytes microcontroller memory. Since size of
buffer is significantly smaller a overflow takes place allowing an attacker
to extract memory contents past the buffer boundary.

Besides application layer also MCUBoot builds with Zephyr USB device stack enabled
are affected. Furthermore along memory readout past buffer bounaries the DFU class
may also be exloited to achive buffer overflow write.

For best results it is recommended to use a libusb build with MAX_CTRL_BUFFER_LENGTH
size increased from default 4096 bytes to 0xFFFF (libusb/os/linux_usbfs.h).

Security Advisory: https://github.com/zephyrproject-rtos/zephyr/security/advisories/GHSA-c3gr-hgvr-f363
