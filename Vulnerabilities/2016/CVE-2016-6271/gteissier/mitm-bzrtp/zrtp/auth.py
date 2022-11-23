#!/usr/bin/python3

# Project     : pyzrtp
# Copyright (C) 2017 Orange
# All rights reserved.
# This software is distributed under the terms and conditions of the 'BSD 3-Clause'
# license which can be found in the file 'LICENSE' in this package distribution.

import hmac
import hashlib
from functools import partial
from binascii import hexlify

class UnsupportedAuthType(Exception): pass

class HMAC_SHA:
  def __init__(self, key):
    self.m = hmac.new(key, digestmod=hashlib.sha256)

  def update(self, data): self.m.update(data)
  def digest(self): return self.m.digest()
  def hexdigest(self): return hexlify(self.digest())

AUTHS = (
# HMAC-SHA1 32 bits
  (b'HS32', lambda k: HMAC_SHA(k)),
# HMAC-SHA1 80 bits
  (b'HS80', lambda k: HMAC_SHA(k)),
)

def get(name):
  auth = None
  for (aname, constructor) in AUTHS:
    if aname == name: return constructor
  if auth is None: raise UnsupportedAuthType(name)

if __name__ == '__main__':
  def auth(name, key, data):
    m = get(name)(key)
    m.update(data)
    return m.hexdigest()
