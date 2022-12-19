#!/usr/bin/env python3

import struct as s

def info(*args, **kwargs):
    print('[*]',*args, **kwargs)

def err(*args, **kwargs):
    print('[!]',*args, **kwargs)

def createPkt(msg_type, payload, length=None, unk1=0, unk2=0, unk3=0x04030201, unk4=0x04030201, unk5=0):
    buf = s.pack('<I', 0xffeeddcc)
    buf += s.pack('<I', msg_type)
    buf += s.pack('<I', unk1)
    if not length:
        length = len(payload)
    length += 0x20
    info('In hdr length', length)
    buf += s.pack('<I', length)
    buf += s.pack('<I', unk2)
    #   subhdr
    buf += s.pack('<I', unk3)   #   deviceId
    buf += s.pack('<I', unk4)   #   userId
    buf += s.pack('<I', unk5)   #   magic
    buf += payload
    info('Pkt length', len(buf))
    return buf

def readResp(r):
    hdr = r.read(0x20)
    info('Resp recv',repr(hdr))
    (hdr1, typ2, unk1, len3) = s.unpack('<IIII', hdr[:16])
    info(hex(hdr1), hex(typ2), "len:",len3)
    payload = r.read(len3-0x20)
    info('Resp body',repr(payload))
    return hdr+payload

def sendRebootCmd(r):
    buf = b""
    r.send(createPkt(0x4f32, buf))

def sendFtpPkt(r, u=b'username' + b'\0'*8, p=b'password' + b'\0'*8, filename=b'filename' + b'\0'*8):
    buf = u
    buf += p
    buf += filename
    r.send(createPkt(0x4f5b, buf))
