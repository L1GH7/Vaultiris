#!/usr/bin/python3

# Project     : pyzrtp
# Copyright (C) 2017 Orange
# All rights reserved.
# This software is distributed under the terms and conditions of the 'BSD 3-Clause'
# license which can be found in the file 'LICENSE' in this package distribution.

import hmac
import hashlib
from functools import partial
from struct import pack

class UnsupportedHashType(Exception): pass

HASHES = (
# SHA-256
  (b'S256', hashlib.sha256),
# SHA-384
  (b'S384', hashlib.sha384),
)

def get(name):
  hash = None
  for (hname, constructor) in HASHES:
    if hname == name:  return constructor
  if hash is None: raise UnsupportedHashType(name)

# only works for sha256 !!!
# to be tested: replace hashlib.sha256 by one of negociated hash
def get_kdf(alg):
  def _kdf(key, label, context, l):
    assert(l % 8 == 0)
    assert(label is not None and len(label) > 0)

    lbytes = l>>3
    return (hmac.new(key, b'\x00\x00\x00\x01' + label + b'\x00' +
      context + pack('!I', l), alg).digest())[:lbytes]

  return _kdf

if __name__ == '__main__':
  from binascii import unhexlify as ux

  def hash(name, value):
    m = get(name)()
    m.update(value)
    return m.hexdigest()

  assert(hash(b'S256', ux('bb12d0c462f29eaa31570ec83537a763678fe632a4e8758a2131e103af45d3e7')) == 'f026775816c4dc1c7dfe8d42a610a5f92ca733a1dcfe85667bf9e136921718bf')
  assert(hash(b'S256', ux('f026775816c4dc1c7dfe8d42a610a5f92ca733a1dcfe85667bf9e136921718bf')) == '418841a7ab2d8f314418f293ad68c3eb0b66acebdd72e161e2c78e83eb3c4f99')

  assert(hash(b'S384', ux('505a007544485061727432207e42be67b021243cd63c445f84a35ff6aa5d509d167d27c1875f510ca9c73a7084d7c25c6f0052276ffe3f8a641824358041f32546f9a9ddda839effdeea6a4652302bc8a0c8861dd1fc33f57c6c11fbfe213d5b1e094a3ed21b2b33db4df374541be0a60d97d171cac1b76daf894db957a23a2cc723ac3648ecf22692821b31ce4fe6483d77e10b270693214f4fba9a16a775f6640d6596f79075d75546b79afbb6e87cf3d9a91a4ed73e170a6111e02a38b5415dad83ba7a75884cf3807e0fdd76b83f05e7cdfcafdbad232dd2afafac23a4b59b12253a0542fd58fad6c8b34f0383a09257d2b591fd504aaf803755c79f95592bef4d0e4042f8d071da83cae3c887c19aeae4fcb6ffb21d167725ab6caa943480a50602fbafefc5f251248670c70b6338c8b2bf4d88919eaa29747f4d8a92e930950d5c12eb543bf5b3e4f30dac7ddb70eefc0e16b7c4b44e0c426750aa8c2d27d2c1255a430ac81143bf55ef7f614f493bd3351431b63889f0aa22f2a0f79ff34d2047a4fa7c5a433e8348bf2578089ee485770f9b9c03fb059b1496142e51b4c6d5c5318303cb4650c4222404450dd547d9e62a375207be7d76064595d2c3904e51833c0f1fa90994744b2071c6e78d5d13ab505a002148656c6c6f202020312e3130474e55205a52545020342e352e302020779ed532598e36c6667d85a4d8077fdd0d42e6e8bddac08aadd8a78cd0e3d0520500565c47bc8ee532e242660002242153333834534b4e3341455333324653334853333248533830534b3332534b36344448326b4448336b423235369a31be4095bd5a96')) == '32c761db53f3764507f5f2cfc30de73e32cc2ba8b2fb6de1ad455cf59c654f50974212d47c07bd8bf490e6e016a8de6a')

  KDF = get_kdf(hashlib.sha256)
  x = KDF(ux('6eaf9bc8ccd3e43bc53f482fe82010e2a2ad3bbc63d53cdc59a0d538f5610f35'),
    ux('496e69746961746f722053525450206d6173746572206b6579'),
    ux('53f76dadac50d1e71c26ddf1d365f040179f9ef8915c1f61021d0a09a1e22857dcd368880cc96446b67a3a364ea4280d44b793343294f425'),
    256)
  assert(x == ux('6cb43469922a1842d947f29d7b0e769774ec31504f9e9b46618d198859bb6017'))

