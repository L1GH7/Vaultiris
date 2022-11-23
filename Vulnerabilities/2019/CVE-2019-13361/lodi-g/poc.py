#!/usr/bin/env python3

from argparse import ArgumentParser
import socket
import time


def pad(nb, n):
    return str(nb).zfill(n)


def set_wifi(s, ssid, pswd):
    payload = "%s%s%s%s%s%s%s%s" % (
        "CGWPCS48",                     # Protocol
        pad(len(ssid), 2),              # SSID len
        ssid,                           # SSID
        "3",                            # ???
        str(len(pswd)),                 # Password len
        pswd,                           # Password
        time.strftime("%Y%m%d%H%M%S"),  # Time
        "\r\n"
    )

    payload = payload.encode("utf-8")

    print(f"set_wifi: sending:\t{payload}")
    s.sendall(payload)
    reply = s.recv(1024)
    print(f"set_wifi: receiving:\t{reply}")
    device_id = reply[12:22]
    print(f"set_wifi: device_id:\t{device_id}")
    return device_id


def disarm(s, device_id):
    payload = "%s%s%s%s" % (
        "CGWPCS53",          # Protocol
        "0000",              # ???
        device_id.decode(),  # Device ID
        "0"                  # Mode: 0/1/2 ; Disarm/Arm/home
    )
    payload = payload.encode("utf-8")

    print(f"disarm: sending:\t{payload}")
    s.sendall(payload)
    reply = s.recv(1024)
    print(f"disarm: receiving:\t{reply}")


def main(args):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((args.ip, args.port))
        device_id = set_wifi(s, args.wifi_ssid, args.wifi_password)
        disarm(s, device_id)


if __name__ == '__main__':
    ap = ArgumentParser()
    ap.add_argument("ip")
    ap.add_argument("-p", "--port", type=int, default=60003)
    ap.add_argument("wifi_ssid")
    ap.add_argument("wifi_password")

    args = ap.parse_args()
    main(args)
