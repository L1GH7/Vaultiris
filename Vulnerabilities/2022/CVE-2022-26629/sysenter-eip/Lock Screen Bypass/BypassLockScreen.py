# /usr/bin/env python3

#
# Lock Screen Bypass for Soroush 1.0.30
# Windows:
#   - Filename : Soroush+
#   - Architecutre: Windows x86
#   - SHA256: c7d3a2c34bab3b1e12bf0c4edb46e26ae625891b671311a9503858628881147c
#
# Linux:
#   - Filename : Soroush+
#   - Architecutre: ELF 64-bit
#   - SHA256: fd9eb708b48029f8e0469f3c0ed06bfdd4eb504f245344c7d8d7670754fe51b6
#

import os
import time
from sys import platform

process_name = ""
executable = ""
patches = []

if "linux" in platform:
    process_name = "Soroush+"
    executable = "Soroush+"
    patches = [
        {
            "name": "patch_1",
            "size": 9,
            "offset": 0x21CB73,
            "original": b"\x40\x84\xED\x0F\x84\x54\x04\x00\x00",
            "patched": b"\x90" * 9,
        },
        {
            "name": "patch_2",
            "size": 9,
            "offset": 0x21A487,
            "original": b"\x45\x84\xED\x0F\x84\x40\x01\x00\x00",
            "patched": b"\x90" * 9,
        },
    ]
else:
    process_name = "Soroush+.exe"
    executable = "Soroush+.exe"

    patches = [
        {
            "name": "patch_1",
            "size": 6,
            "offset": 0x157A08,
            "original": b"\x80\x7d\xf3\x00\x74\x16",
            "patched": b"\x90" * 6,
        },
        {
            "name": "patch_2",
            "size": 6,
            "offset": 0x1599F4,
            "original": b"\x80\x7d\x73\x00\x74\x3c",
            "patched": b"\x90" * 6,
        },
    ]


def kill_by_process_name(name):
    if "linux" in platform:
        os.system("killall -9 " + name)
    else:
        os.system("taskkill /f /im " + name)

    time.sleep(0.5)


def start_process(executable):
    print("[*] Restarting...")
    if "linux" in platform:
        os.system(executable)
    else:
        os.startfile(executable)


def patch_all(patches=None):
    """
    Applies all patches
    :param patches: list (or tuple) of patch info dictionaries
    """
    if patches is None:
        return

    with open(executable, "r+b") as f:
        for patch in patches:
            f.seek(patch["offset"])
            data = f.read(patch["size"])

            if data == patch["patched"]:
                f.seek(patch["offset"])
                f.write(patch["original"])
                print("[+] {} successfully reverted.".format(patch["name"]))
            else:
                f.seek(patch["offset"])
                f.write(patch["patched"])
                print("[+] {} successfully applied.".format(patch["name"]))


def main():
    """
    Main function
    """
    kill_by_process_name(process_name)
    patch_all(patches=patches)
    start_process(executable)


if __name__ == "__main__":
    main()
