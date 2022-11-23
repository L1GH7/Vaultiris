# Proof of Concept for CVE-2021-37740

## Table of Contents

- [Introduction](#introduction)
- [The Vulnerability](#the-vulnerability)
- [Executing the PoC](#executing-the-poc)
- [Remediation](#remediation)
- [Coordinated Vulnerability Disclosure](#coordinated-vulnerability-disclosure)
- [License](#license)
- [Change Log](#change-log)

## Introduction

[CVE-2021-37740](https://nvd.nist.gov/vuln/detail/CVE-2021-37740) is a denial of service (DoS) vulnerability that affects firmware v3.0.3 of the KNXnet/IP Secure router SCN-IP100.03 and interface SCN-IP000.03 by MDT. 
A specially crafted KNXnet/IP Secure frame results in a device that is unresponsive to further requests, requiring a reboot the restore normal operations.
This respository contains a proof of concept that demonstrates how the bug can be triggered, which was originally developed for the coordinated vulnerability disclosure.

## The Vulnerability

An adversary can exploit the vulnerability by establishing a TCP connection with the target device, followed by sending a `SESSION_REQUEST` frame with a modified total length field, containing a value of `0x0259` or higher. 
Devices affected by the vulnerability will no longer respond to KNXnet/IP Secure frames in any connection. The `SESSION_REQUEST` is the first frame sent during the handshake of the KNXnet/IP Secure unicast protocol, as specified by ISO 22510:2019. 
It is supposed to have a fixed length of 46 bytes. The standard requires that frames with a deviating length are discarded.

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="./imgs/session-request-dark-mode.svg">
  <source media="(prefers-color-scheme: light)" srcset="./imgs/session-request.svg">
  <img alt="Modified SESSION_REQUEST frame." src="./imgs/session-request.svg">
</picture>

## Executing the Proof of Concept

Python 3 is required for the scripts provided in this repository. The `healthcheck.py` is for testing whether the device is still responsive. 
It establishes a TCP connection with the target device and attempts to start the KNXnet/IP Secure unicast handshake. 
If the device sends a reply, it will be print it as hex to `stdout`.
The `poc.py` tries to trigger the DoS vulnerability by sending the invalid `SESSION_REQUEST` frame. If a device is vulnerable, subsequent executions of the `healthcheck.py` will not yield a reply, unless the the target device is rebooted.

The scripts can be executed with the following commands, where `<insert IP address>` has to be replaced with the actual IP address (e.g. `192.168.2.137`).

`python3 healthcheck.py -t <insert IP address>`

`python3 poc.py -t <insert IP address>`

On Windows the alias for Python may have to be changed from `python3` to `python`. If the target device implements the control endpoint on a port other than `3671`, then the port in the script has to be adjusted accordingly. 

## Remediation

The vulnerability was fixed in firmware v3.0.4, as documented by the [change log](https://www.mdt.de/download/MDT_CL_SCN_IP_03_IP_Interface_Router.pdf) for the MDT firmware. Installing the most recent firmware remediates the issue. Instructions for the firmware update and the `.hex` file are provided on [MDT's website](https://www.mdt.de/produkte/produktdetail.html?tx_mdtproducts_detail%5Baction%5D=detail&tx_mdtproducts_detail%5Bcontroller%5D=Productseries&tx_mdtproducts_detail%5Bseries%5D=61&cHash=b5790b112aaa7ae5fe450726ea614983).

## Coordinated Vulnerability Disclosure

- 2021-07-06 - Issue reported to MDT
- 2021-07-07 - Investigation started by MDT
- 2021-07-28 - MDT confirmed the bug and provided a patched firmware for testing
- 2021-08-01 - Confirmation provided to MDT that the bug was fixed
- 2021-08-02 - MDT released firmware v3.0.4
- 2022-04-01 - Public disclosure

## License

The proof of concept is distributed under the [MIT license](./LICENSE).

## Change Log

### 1.0.0 - 2022-04-01
**Download:**
- [Soure Code](https://github.com/robertguetzkow/CVE-2021-37740/archive/refs/tags/v1.0.0.zip)

**Changes:**
- Initial version
