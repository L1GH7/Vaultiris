#!/usr/bin/python3

# Project     : pyzrtp
# Copyright (C) 2017 Orange
# All rights reserved.
# This software is distributed under the terms and conditions of the 'BSD 3-Clause'
# license which can be found in the file 'LICENSE' in this package distribution.

from Crypto.Cipher import AES
from collections import namedtuple

class UnsupportedCipherType(Exception): pass

# TODO: implement TwoFish CFB

Cipher = namedtuple('Cipher', 'name constructor keybits')

class Cipher_AES:
  def __init__(self, key, iv, key_bits):
    assert(key_bits % 8 == 0)
    self.key_bytes = key_bits>>3

    assert(len(key) == self.key_bytes)
    self.c = AES.new(key, AES.MODE_CFB, iv, segment_size=128)

  def encrypt(self, data):
    padded_data = data
    while len(padded_data) % AES.block_size != 0:
      padded_data += b'\x00'

    cipher = self.c.encrypt(padded_data)

    return cipher[:len(data)]

  def decrypt(self, data):
    padded_data = data
    while len(padded_data) % AES.block_size != 0:
      padded_data += b'\x00'

    plain = self.c.decrypt(padded_data)

    return plain[:len(data)]

CIPHERS = (
  Cipher(b'AES3', lambda k, iv: Cipher_AES(k, iv, 256), 256), # AES-CFB with 256 bits key
  Cipher(b'AES1', lambda k, iv: Cipher_AES(k, iv, 128), 128), # AES-CFB with 128 bits key
)

def get(name):
  for c in CIPHERS:
    if c.name == name: return c
  raise UnsupportedCipherType(name)

if __name__ == '__main__':
  from binascii import hexlify
  from binascii import unhexlify as ux

  def cipher(name, key, iv, data):
    c = get(name)
    assert(c.keybits<<3 == len(key))
    instance = c.constructor(key, iv)
    return hexlify(instance.encrypt(data))
