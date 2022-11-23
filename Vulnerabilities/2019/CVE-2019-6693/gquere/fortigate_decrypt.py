#!/usr/bin/env python3
from Cryptodome.Cipher import AES
import base64
import sys

key = b'Mary had a littl'

data = base64.b64decode(sys.argv[1])

iv = data[0:4] + b'\x00' * 12
ct = data[4:]

cipher = AES.new(key, iv=iv, mode=AES.MODE_CBC)
pt = cipher.decrypt(ct)

try:
    print(pt.decode())
except:
    print(pt)
