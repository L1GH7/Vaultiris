# MIT License
#
# Copyright (c) 2021 Robert Gützkow
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import socket
import argparse


def health_check(target_ip):
    '''
    Attempt to start a KNX IP Secure handshake and check if a reply is received. When the denial of service attack
    from the poc.py is successful, no reply should be received anymore, unless the device is rebooted.
    :param target_ip: IP address of the target device
    '''
    port = 3671
    buffer_size = 56  # Length of a SESSION_RESPONSE frame
    payload = bytearray([
        # KNX IP header
        0x06,  # Header length (1 byte)
        0x10,  # Protocol version (1 byte)
        0x09,  # Service type identifier for SESSION_REQUEST (2 bytes)
        0x51,
        0x00,  # Total length (2 bytes)
        0x2e,
        # HPAI control endpoint
        0x08,  # Structure length (1 byte)
        0x02,  # Host protocol code for TCP (1 byte)
        0x00,  # IPv4 address for client's control endpoint (4 bytes) set all zero for route back
        0x00,
        0x00,
        0x00,
        0x00,  # Port number (2 bytes) set to all zero for route back
        0x00
    ])
    payload.extend(bytes([0x00] * 32))  # Diffie-Hellman client public value X (32 bytes)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((target_ip, port))
    s.sendall(payload)
    reply = s.recv(buffer_size)  # Should provide at least 1 byte as reply if successful
    print(reply.hex())
    s.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Test if KNX IP Secure device is still reachable")
    parser.add_argument("-t",
                        "--target",
                        type=str,
                        help="IP address of the target device",
                        required=True)
    args = parser.parse_args()
    health_check(args.target)
