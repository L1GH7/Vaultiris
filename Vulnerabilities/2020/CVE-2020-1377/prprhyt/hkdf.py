'''
=========================
# HKDF functions from https://github.com/aiortc/aioquic/blob/c2673a5a64dd74dd8aa056cfc5a325c29cd20f55/src/aioquic/tls.py

Copyright (c) 2019 Jeremy Lainé.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of aioquic nor the names of its contributors may
      be used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''

from typing import (
    Optional
)
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.hkdf import HKDF, HKDFExpand
from cryptography.hazmat.primitives import hmac

def hkdf_label(label: bytes, hash_value: bytes, length: int) -> bytes:
    full_label = b"tls13 " + label
    return (
        struct.pack("!HB", length, len(full_label))
        + full_label
        + struct.pack("!B", len(hash_value))
        + hash_value
    )

def hkdf_expand_label(
    algorithm: hashes.HashAlgorithm,
    secret: bytes,
    label: bytes,
    hash_value: bytes,
    length: int,
) -> bytes:
    return HKDFExpand(
        algorithm=algorithm,
        length=length,
        info=hkdf_label(label, hash_value, length),
        backend=default_backend(),
    ).derive(secret)


def hkdf_extract(
    algorithm: hashes.HashAlgorithm, salt: bytes, key_material: bytes
) -> bytes:
    h = hmac.HMAC(salt, algorithm, backend=default_backend())
    h.update(key_material)
    return h.finalize()


class KeySchedule:
    def __init__(self, algorithm: hashes.HashAlgorithm):
        self.algorithm = algorithm
        self.generation = 0
        self.hash = hashes.Hash(self.algorithm, default_backend())
        self.hash_empty_value = self.hash.copy().finalize()
        self.secret = bytes(self.algorithm.digest_size)

    def certificate_verify_data(self, context_string: bytes) -> bytes:
        return b" " * 64 + context_string + b"\x00" + self.hash.copy().finalize()

    def finished_verify_data(self, secret: bytes) -> bytes:
        hmac_key = hkdf_expand_label(
            algorithm=self.algorithm,
            secret=secret,
            label=b"finished",
            hash_value=b"",
            length=self.algorithm.digest_size,
        )

        h = hmac.HMAC(hmac_key, algorithm=self.algorithm, backend=default_backend())
        h.update(self.hash.copy().finalize())
        return h.finalize()

    def derive_secret(self, label: bytes) -> bytes:
        return hkdf_expand_label(
            algorithm=self.algorithm,
            secret=self.secret,
            label=label,
            hash_value=self.hash.copy().finalize(),
            length=self.algorithm.digest_size,
        )

    def extract(self, key_material: Optional[bytes] = None) -> None:
        if key_material is None:
            key_material = bytes(self.algorithm.digest_size)

        if self.generation:
            self.secret = hkdf_expand_label(
                algorithm=self.algorithm,
                secret=self.secret,
                label=b"derived",
                hash_value=self.hash_empty_value,
                length=self.algorithm.digest_size,
            )

        self.generation += 1
        self.secret = hkdf_extract(
            algorithm=self.algorithm, salt=self.secret, key_material=key_material
        )

    def update_hash(self, data: bytes) -> None:
        self.hash.update(data)


'''
=========================
HKDF functions end of line
'''