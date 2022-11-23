#!/usr/bin/python3

# Project     : pyzrtp
# Copyright (C) 2017 Orange
# All rights reserved.
# This software is distributed under the terms and conditions of the 'BSD 3-Clause'
# license which can be found in the file 'LICENSE' in this package distribution.

from binascii import unhexlify, hexlify
from struct import pack, unpack
from collections import namedtuple
import io

from functools import reduce

CRC_TAB = (
    0x00000000, 0xf26b8303, 0xe13b70f7, 0x1350f3f4,
    0xc79a971f, 0x35f1141c, 0x26a1e7e8, 0xd4ca64eb,
    0x8ad958cf, 0x78b2dbcc, 0x6be22838, 0x9989ab3b,
    0x4d43cfd0, 0xbf284cd3, 0xac78bf27, 0x5e133c24,
    0x105ec76f, 0xe235446c, 0xf165b798, 0x030e349b,
    0xd7c45070, 0x25afd373, 0x36ff2087, 0xc494a384,
    0x9a879fa0, 0x68ec1ca3, 0x7bbcef57, 0x89d76c54,
    0x5d1d08bf, 0xaf768bbc, 0xbc267848, 0x4e4dfb4b,
    0x20bd8ede, 0xd2d60ddd, 0xc186fe29, 0x33ed7d2a,
    0xe72719c1, 0x154c9ac2, 0x061c6936, 0xf477ea35,
    0xaa64d611, 0x580f5512, 0x4b5fa6e6, 0xb93425e5,
    0x6dfe410e, 0x9f95c20d, 0x8cc531f9, 0x7eaeb2fa,
    0x30e349b1, 0xc288cab2, 0xd1d83946, 0x23b3ba45,
    0xf779deae, 0x05125dad, 0x1642ae59, 0xe4292d5a,
    0xba3a117e, 0x4851927d, 0x5b016189, 0xa96ae28a,
    0x7da08661, 0x8fcb0562, 0x9c9bf696, 0x6ef07595,
    0x417b1dbc, 0xb3109ebf, 0xa0406d4b, 0x522bee48,
    0x86e18aa3, 0x748a09a0, 0x67dafa54, 0x95b17957,
    0xcba24573, 0x39c9c670, 0x2a993584, 0xd8f2b687,
    0x0c38d26c, 0xfe53516f, 0xed03a29b, 0x1f682198,
    0x5125dad3, 0xa34e59d0, 0xb01eaa24, 0x42752927,
    0x96bf4dcc, 0x64d4cecf, 0x77843d3b, 0x85efbe38,
    0xdbfc821c, 0x2997011f, 0x3ac7f2eb, 0xc8ac71e8,
    0x1c661503, 0xee0d9600, 0xfd5d65f4, 0x0f36e6f7,
    0x61c69362, 0x93ad1061, 0x80fde395, 0x72966096,
    0xa65c047d, 0x5437877e, 0x4767748a, 0xb50cf789,
    0xeb1fcbad, 0x197448ae, 0x0a24bb5a, 0xf84f3859,
    0x2c855cb2, 0xdeeedfb1, 0xcdbe2c45, 0x3fd5af46,
    0x7198540d, 0x83f3d70e, 0x90a324fa, 0x62c8a7f9,
    0xb602c312, 0x44694011, 0x5739b3e5, 0xa55230e6,
    0xfb410cc2, 0x092a8fc1, 0x1a7a7c35, 0xe811ff36,
    0x3cdb9bdd, 0xceb018de, 0xdde0eb2a, 0x2f8b6829,
    0x82f63b78, 0x709db87b, 0x63cd4b8f, 0x91a6c88c,
    0x456cac67, 0xb7072f64, 0xa457dc90, 0x563c5f93,
    0x082f63b7, 0xfa44e0b4, 0xe9141340, 0x1b7f9043,
    0xcfb5f4a8, 0x3dde77ab, 0x2e8e845f, 0xdce5075c,
    0x92a8fc17, 0x60c37f14, 0x73938ce0, 0x81f80fe3,
    0x55326b08, 0xa759e80b, 0xb4091bff, 0x466298fc,
    0x1871a4d8, 0xea1a27db, 0xf94ad42f, 0x0b21572c,
    0xdfeb33c7, 0x2d80b0c4, 0x3ed04330, 0xccbbc033,
    0xa24bb5a6, 0x502036a5, 0x4370c551, 0xb11b4652,
    0x65d122b9, 0x97baa1ba, 0x84ea524e, 0x7681d14d,
    0x2892ed69, 0xdaf96e6a, 0xc9a99d9e, 0x3bc21e9d,
    0xef087a76, 0x1d63f975, 0x0e330a81, 0xfc588982,
    0xb21572c9, 0x407ef1ca, 0x532e023e, 0xa145813d,
    0x758fe5d6, 0x87e466d5, 0x94b49521, 0x66df1622,
    0x38cc2a06, 0xcaa7a905, 0xd9f75af1, 0x2b9cd9f2,
    0xff56bd19, 0x0d3d3e1a, 0x1e6dcdee, 0xec064eed,
    0xc38d26c4, 0x31e6a5c7, 0x22b65633, 0xd0ddd530,
    0x0417b1db, 0xf67c32d8, 0xe52cc12c, 0x1747422f,
    0x49547e0b, 0xbb3ffd08, 0xa86f0efc, 0x5a048dff,
    0x8ecee914, 0x7ca56a17, 0x6ff599e3, 0x9d9e1ae0,
    0xd3d3e1ab, 0x21b862a8, 0x32e8915c, 0xc083125f,
    0x144976b4, 0xe622f5b7, 0xf5720643, 0x07198540,
    0x590ab964, 0xab613a67, 0xb831c993, 0x4a5a4a90,
    0x9e902e7b, 0x6cfbad78, 0x7fab5e8c, 0x8dc0dd8f,
    0xe330a81a, 0x115b2b19, 0x020bd8ed, 0xf0605bee,
    0x24aa3f05, 0xd6c1bc06, 0xc5914ff2, 0x37faccf1,
    0x69e9f0d5, 0x9b8273d6, 0x88d28022, 0x7ab90321,
    0xae7367ca, 0x5c18e4c9, 0x4f48173d, 0xbd23943e,
    0xf36e6f75, 0x0105ec76, 0x12551f82, 0xe03e9c81,
    0x34f4f86a, 0xc69f7b69, 0xd5cf889d, 0x27a40b9e,
    0x79b737ba, 0x8bdcb4b9, 0x988c474d, 0x6ae7c44e,
    0xbe2da0a5, 0x4c4623a6, 0x5f16d052, 0xad7d5351,
)

def cksum(data):
  crc = 0xffffffff

  for c in data:
    crc = ((crc>>8)^CRC_TAB[(crc^c) & 0xff])

  crc = ~crc

  bytes = [((crc>>x) & 0xff)<<(24-x) for x in range(0, 32, 8)]
  return reduce(lambda x,y: x|y, bytes)

U16FMT = 'H'
U32FMT = 'I'

def decode_section(f, elm_size, count):
  elms = []

  for i in range(0, count):
    data = f.read(elm_size)
    assert(len(data) == elm_size)
    elms.append(data)

  return elms

def encode_section(f, elms):
  elm_size = None

  for elm in elms:
    assert(elm_size is None or len(elm) == elm_size)
    elm_size = len(elm)

    f.write(elm)

Hello = namedtuple('Hello',
  'version clt h3 zid S M P hashes ciphers auths keys sass mac')

def decode_hello(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'Hello   ')

  version = f.read(4)
  assert(len(version) == 4)
  assert(version == b'1.10')

  clt = f.read(16)
  assert(len(clt) == 16)

  h3 = f.read(32)
  assert(len(h3) == 32)

  zid = f.read(12)
  assert(len(zid) == 12)

  data = f.read(4)
  assert(len(data) == 4)

  (flags, a) = unpack('!' + 2*U16FMT, data)
  S = (flags & 0x4000)>>14
  M = (flags & 0x2000)>>13
  P = (flags & 0x1000)>>12
  hc = (flags & 0x000f)
  cc = (a & 0xf000)>>12
  ac = (a & 0x0f00)>>8
  kc = (a & 0x00f0)>>4
  sc = (a & 0x000f)

  hashes = decode_section(f, 4, hc)
  ciphers = decode_section(f, 4, cc)
  auths = decode_section(f, 4, ac)
  keys = decode_section(f, 4, kc)
  sass = decode_section(f, 4, sc)

  mac = f.read(8)
  assert(len(mac) == 8)

  return Hello(version, clt, h3, zid, S, M, P, hashes, ciphers, auths, keys, sass, mac)

def encode_hello(m):
  assert(type(m) == Hello)

  b = io.BytesIO()

  content = io.BytesIO()

  content.write(m.version)
  content.write(m.clt)
  content.write(m.h3)
  content.write(m.zid)

  hc = len(m.hashes)
  cc = len(m.ciphers)
  ac = len(m.auths)
  kc = len(m.keys)
  sc = len(m.sass)

  flags = (m.S<<14)|(m.M<<13)|(m.P<<12)|(hc)
  a = (cc<<12)|(ac<<8)|(kc<<4)|(sc)

  content.write(pack('!' + 2*U16FMT, flags, a))

  encode_section(content, m.hashes)
  encode_section(content, m.ciphers)
  encode_section(content, m.auths)
  encode_section(content, m.keys)
  encode_section(content, m.sass)

  # write MAC with a placeholder
  content.write(b'\x00'*8)

  length = len(content.getvalue()) + 8 + 4
  length >>= 2

  b.write(pack('!' + 2*U16FMT, 0x505a, length))
  b.write(b'Hello   ')

  b.write(content.getvalue())

  return b.getvalue()


HelloAck = namedtuple('HelloAck', '')

def decode_hello_ack(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)
  assert(length == 3)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'HelloACK')

  return HelloAck()

def encode_hello_ack():
  b = io.BytesIO()

  b.write(pack('!' + 2*U16FMT, 0x505a, 3))
  b.write(b'HelloACK')

  return b.getvalue()

DHCommit = namedtuple('DHCommit', 'h2 zid hash cipher auth key sas hvi mac')
MultiCommit = namedtuple('MultiCommit',
  'h2 zid hash cipher auth key sas nonce mac')
PresharedCommit = namedtuple('PresharedCommit',
  'h2 zid hash cipher auth key sas nonce keyid mac')

def decode_commit(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)
  assert(length in [25, 27, 29])

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'Commit  ')

  h2 = f.read(32)
  assert(len(h2) == 32)

  zid = f.read(12)
  assert(len(zid) == 12)

  hash = f.read(4)
  assert(len(hash) == 4)

  cipher = f.read(4)
  assert(len(cipher) == 4)

  auth = f.read(4)
  assert(len(auth) == 4)

  key = f.read(4)
  assert(len(key) == 4)

  sas = f.read(4)
  assert(len(sas) == 4)

  hvi = None
  nonce = None
  keyid = None

  if sas == b'Mult':
    nonce = f.read(16)
    assert(len(nonce) == 16)
  elif sas == b'Prsh':
    nonce = f.read(16)
    assert(len(nonce) == 16)

    keyid = f.read(8)
    assert(len(keyid) == 8)
  else:
    hvi = f.read(32)
    assert(len(hvi) == 32)

  mac = f.read(8)
  assert(len(mac) == 8)

  if sas == b'Mult':
    return MultiCommit(h2, zid, hash, cipher, auth, key, sas, nonce, mac)
  elif sas == b'Prsh':
    return PresharedCommit(h2, zid, hash, cipher, auth, key, sas,
      nonce, keyid, mac)
  else:
    return DHCommit(h2, zid, hash, cipher, auth, key, sas, hvi, mac)

KEY_AGREEMENT_VALUES = {
  # name: (total length, pv length)
  'DH3k': (117, 96),
  'DH2k': (85, 64),
  'EC25': (37, 16),
  'EC38': (45, 24),
  'E255': (29, 8),
  'E414': (47, 26),
}

def encode_dhcommit(m):
  assert(type(m) == DHCommit)

  b = io.BytesIO()

  content = io.BytesIO()

  content.write(m.h2)
  content.write(m.zid)
  content.write(m.hash)
  content.write(m.cipher)
  content.write(m.auth)
  content.write(m.key)
  content.write(m.sas)
  content.write(m.hvi)
  content.write(b'\x00'*8)

  length = len(content.getvalue()) + 8 + 4
  length >>= 2

  b.write(pack('!' + 2*U16FMT, 0x505a, length))
  b.write(b'Commit  ')

  b.write(content.getvalue())

  return b.getvalue()

DHPart1 = namedtuple('DHPart1',
  'h1 rs1idr rs2idr auxsecretidr pbxsecretidr pvr mac')

def decode_dhpart1(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'DHPart1 ')

  h1 = f.read(32)
  assert(len(h1) == 32)

  rs1idr = f.read(8)
  assert(len(rs1idr) == 8)

  rs2idr = f.read(8)
  assert(len(rs2idr) == 8)

  auxsecretidr = f.read(8)
  assert(len(auxsecretidr) == 8)

  pbxsecretidr = f.read(8)
  assert(len(pbxsecretidr) == 8)

  pvr = None
  for name in KEY_AGREEMENT_VALUES:
    (total_length, dhlength) = KEY_AGREEMENT_VALUES[name]
    if total_length == length:
      pvr = f.read(dhlength*4)
      assert(len(pvr) == dhlength*4)
      break
  assert(pvr is not None)

  mac = f.read(8)
  assert(len(mac) == 8)

  return DHPart1(h1, rs1idr, rs2idr, auxsecretidr, pbxsecretidr, pvr, mac)

def encode_dhpart1(m):
  assert(type(m) == DHPart1)

  b = io.BytesIO()

  content = io.BytesIO()

  content.write(m.h1)
  content.write(m.rs1idr)
  content.write(m.rs2idr)
  content.write(m.auxsecretidr)
  content.write(m.pbxsecretidr)
  content.write(m.pvr)

  # write MAC with a placeholder
  content.write(b'\x00'*8)

  length = len(content.getvalue()) + 8 + 4
  length >>= 2

  b.write(pack('!' + 2*U16FMT, 0x505a, length))
  b.write(b'DHPart1 ')

  b.write(content.getvalue())

  return b.getvalue()

DHPart2 = namedtuple('DHPart2',
  'h1 rs1idi rs2idi auxsecretidi pbxsecretidi pvi mac')

def decode_dhpart2(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'DHPart2 ')

  h1 = f.read(32)
  assert(len(h1) == 32)

  rs1idi = f.read(8)
  assert(len(rs1idi) == 8)

  rs2idi = f.read(8)
  assert(len(rs2idi) == 8)

  auxsecretidi = f.read(8)
  assert(len(auxsecretidi) == 8)

  pbxsecretidi = f.read(8)
  assert(len(pbxsecretidi) == 8)

  pvi = None
  for name in KEY_AGREEMENT_VALUES:
    (total_length, dhlength) = KEY_AGREEMENT_VALUES[name]
    if total_length == length:
      pvi = f.read(dhlength*4)
      assert(len(pvi) == dhlength*4)
      break
  assert(pvi is not None)

  mac = f.read(8)
  assert(len(mac) == 8)

  return DHPart2(h1, rs1idi, rs2idi, auxsecretidi, pbxsecretidi, pvi, mac)

def encode_dhpart2(m):
  assert(type(m) == DHPart2)

  b = io.BytesIO()

  content = io.BytesIO()

  content.write(m.h1)
  content.write(m.rs1idi)
  content.write(m.rs2idi)
  content.write(m.auxsecretidi)
  content.write(m.pbxsecretidi)
  content.write(m.pvi)

  # write MAC with a placeholder
  content.write(b'\x00'*8)

  length = len(content.getvalue()) + 8 + 4
  length >>= 2

  b.write(pack('!' + 2*U16FMT, 0x505a, length))
  b.write(b'DHPart2 ')

  b.write(content.getvalue())

  return b.getvalue()

Confirm = namedtuple('Confirm', 'confirm_mac cfb_iv encrypted')

def decode_confirm(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'Confirm1' or type_block == b'Confirm2')

  confirm_mac = f.read(8)
  assert(len(confirm_mac) == 8)

  cfb_iv = f.read(16)
  assert(len(cfb_iv) == 16)

  encrypted = f.read()

  return Confirm(confirm_mac, cfb_iv, encrypted)

def encode_confirm(m, type_block):
  assert(type(m) == Confirm)

  b = io.BytesIO()

  content = io.BytesIO()

  content.write(m.confirm_mac)
  content.write(m.cfb_iv)
  content.write(m.encrypted)

  length = len(content.getvalue()) + 8 + 4
  length >>= 2

  b.write(pack('!' + 2*U16FMT, 0x505a, length))
  b.write(type_block)

  b.write(content.getvalue())

  return b.getvalue()

def encode_confirm1(m):
  return encode_confirm(m, b'Confirm1')
def encode_confirm2(m):
  return encode_confirm(m, b'Confirm2')

Conf2Ack = namedtuple('Conf2Ack', '')

def decode_conf2ack(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)
  assert(length == 3)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'Conf2ACK')

  return Conf2Ack()

def encode_conf2ack():
  b = io.BytesIO()

  b.write(pack('!' + 2*U16FMT, 0x505a, 3))
  b.write(b'Conf2ACK')

  return b.getvalue()

Error = namedtuple('Error', 'code')

def decode_error(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)
  assert(length == 4)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'Error   ')

  data = f.read(4)
  (code,) = unpack('!' + U32FMT, data)

  return Error(code)

ErrorAck = namedtuple('ErrorAck', '')

def decode_error_ack(b):
  f = io.BytesIO(b)

  data = f.read(4)
  assert(len(data) == 4)

  (preamble, length) = unpack('!' + 2*U16FMT, data)
  assert(len(b) == 4*length)
  assert(preamble == 0x505a)
  assert(length == 3)

  type_block = f.read(8)
  assert(len(type_block) == 8)
  assert(type_block == b'ErrorACK')

  return ErrorAck()

Confirmation = namedtuple('Confirmation', 'h0 E V A D cache_expiration type_block signature')
Confirmation.__new__.__defaults__ = (None,) * 8

def decode_confirmation(b):
  f = io.BytesIO(b)

  h0 = f.read(32)
  assert(len(h0) == 32)

  data = f.read(4)
  assert(len(data) == 4)

  (a, b, c) = unpack('!' + U16FMT + 'BB', data)
  siglen = (a & 0x0001)<<8 + b
  E = (c & 0x08)>>3
  V = (c & 0x04)>>2
  A = (c & 0x02)>>1
  D = (c & 0x01)>>0

  cache_expiration = f.read(4)
  assert(len(cache_expiration) == 4)

  type_block=b''
  signature = b''
  if siglen != 0:
    type_block = f.read(4)
    assert(len(type_block) == 4)

    signature = f.read(4*siglen-4)
    assert(len(signature) == 4*siglen-4)

  return Confirmation(h0, E, V, A, D, cache_expiration, type_block, signature)

def encode_confirmation(m):
  b = io.BytesIO()

  b.write(m.h0)

  if m.signature:
    assert(len(m.signature) % 4 == 0)
    siglen = (len(m.signature) + 4)//4
  else:
    siglen = 0
  b.write(b'\x00' + pack('!' + U16FMT, siglen))

  flags = (m.E<<3)|(m.V<<2)|(m.A<<1)|m.D
  b.write(pack('!B', flags))

  b.write(pack('!I', m.cache_expiration))

  if m.signature:
    b.write(m.type_block)
    b.write(m.signature)

  return b.getvalue()

Pkt = namedtuple('Pkt',
  'version padding extension seqno cookie src msg raw')
Pkt.__new__.__defaults__ = (None,) * 8

def decode(b):
  assert(len(b) >= 16)

  (flags, seqno, cookie, src) = unpack('!' + 2*U16FMT + 2*U32FMT, b[:12])
  message = b[12:-4]
  (crc,) = unpack('!' + U32FMT, b[-4:])

  computed_crc = cksum(b[:-4])

  version = (flags & 0xc000)>>14
  padding = (flags & 0x2000)>>13
  extension = (flags & 0x1000)>>12

  if version != 0 or cookie != 0x5a525450:
    return None

  assert(crc == computed_crc)

  (preamble, length) = unpack('!' + 2*U16FMT, message[:4])
  assert(len(message) == 4*length)

  type_block = message[4:12]
  assert(len(type_block) == 8)

  msg = None
  if type_block == b'Hello   ':
    msg = decode_hello(message)
  elif type_block == b'HelloACK':
    msg = decode_hello_ack(message)
  elif type_block == b'Commit  ':
    msg = decode_commit(message)
  elif type_block == b'DHPart1 ':
    msg = decode_dhpart1(message)
  elif type_block == b'DHPart2 ':
    msg = decode_dhpart2(message)
  elif type_block == b'Confirm1' or type_block == b'Confirm2':
    msg = decode_confirm(message)
  elif type_block == b'Conf2ACK':
    msg = decode_conf2ack(message)
  elif type_block == b'Error   ':
    msg = decode_error(message)
  elif type_block == b'ErrorACK':
    msg = decode_error_ack(message)
  elif type_block == b'GoClear ':
    pass
  elif type_block == b'ClearACK':
    pass
  elif type_block == b'SASRelay':
    pass
  elif type_block == b'RelayACK':
    pass
  elif type_block == b'Ping    ':
    pass
  elif type_block == b'PingACK ':
    pass

  return Pkt(version, padding, extension, seqno, cookie, src, msg=msg, raw=message)

def encode(p):
  b = io.BytesIO()

  flags = (p.version<<14)|(p.padding<<13)|(p.extension<<12)
  b.write(pack('!' + 2*U16FMT + 2*U32FMT, flags, p.seqno, p.cookie, p.src))

  b.write(p.raw)

  crc = cksum(b.getvalue())
  b.write(pack('!' + U32FMT, crc))

  return b.getvalue()

if __name__ == '__main__':
  class PcapMagicUnknown(Exception): pass

  import sys
  from struct import unpack

  def walk(name):
    f = open(name, 'rb')
    magic = f.read(4)
    assert(len(magic) == 4)

    (magic,) = unpack('=L', magic)
    if magic == 0xa1b2c3d4:
      swapped = '<'
    elif magic == 0xd4c3b2a1:
      swapped = '>'
    else:
      raise PcapMagicUnknown(magic)
      sys.exit(1)

    fhdr = f.read(20)
    assert(len(fhdr) == 20)
    (maj, min, zone, sigfigs, snaplen, dlt) = unpack(swapped + 'HHLLLL', fhdr)

    frames = []

    while True:
      phdr = f.read(16)
      if len(phdr) == 0:
        break
      assert(len(phdr) == 16)
      (sec, usec, caplen, l) = unpack(swapped + 'LLLL', phdr)
      content = f.read(caplen)
      assert(len(content) == caplen)

      yield (sec + usec*0.000001, caplen, bytes(content))

  for arg in sys.argv[1:]:
    print('analyzing %r' % arg)
    for  (ts, caplen, content) in walk(arg):
      content = content[42:]
      pkt = decode(content)
      print(pkt)
