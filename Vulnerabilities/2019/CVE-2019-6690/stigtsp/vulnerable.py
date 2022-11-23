#!/usr/bin/env python3
import gnupg, secrets, sys, json

gpg = gnupg.GPG(gnupghome="/tmp/gpg")

def encrypt_data(password):
    return str(gpg.encrypt("expected message", recipients=False, symmetric=True, passphrase=password))

in_password = sys.stdin.read()

print(encrypt_data(in_password))

