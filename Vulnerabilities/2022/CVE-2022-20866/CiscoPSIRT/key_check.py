#!/usr/bin/env python3
#
# Copyright (c) 2022 by Cisco Systems, Inc.
#

import re
import os
import tempfile
import subprocess
import getpass
import shlex


#
# PrivateKey class provides an abstraction of openssl private key output
#
class PrivateKey:

    def __init__(self, key_str):
        self.key_str = key_str
        self.lines = key_str.splitlines()
        self.key_bytes = 0
        self.bad_key = True
        self.short_key = False
        self.trailing_zero = False
        self.modulus = []
        self.private_exponent = []
        self.prime1 = []
        self.prime2 = []
        self.exponent1 = []
        self.exponent2 = []
        self.coefficient = []
        self.total = 0

    def is_short(self):
        return self.short_key

    def is_bad(self):
        return self.bad_key

    def is_trailing_zero(self):
        return self.trailing_zero

    @staticmethod
    def short_component(component, expected):
        shorted = False

        if len(component) < expected:
            shorted = True

        return shorted

    def short_modulus(self):
        return self.short_component(self.modulus, self.key_bytes)

    def short_private_exponent(self):
        return self.short_component(self.private_exponent, self.key_bytes)

    def short_prime1(self):
        return self.short_component(self.prime1, int(self.key_bytes / 2))

    def short_prime2(self):
        return self.short_component(self.prime2, int(self.key_bytes / 2))

    def short_exponent1(self):
        return self.short_component(self.exponent1, int(self.key_bytes / 2))

    def short_exponent2(self):
        return self.short_component(self.exponent2, int(self.key_bytes / 2))

    def short_coefficient(self):
        return self.short_component(self.coefficient, int(self.key_bytes / 2))

    @staticmethod
    def has_trailing_zero(component):
        trailing_zero = False

        if component[len(component) - 1] == 0:
            trailing_zero = True

        return trailing_zero

    def trailing_zero_modulus(self):
        return self.has_trailing_zero(self.modulus)

    def trailing_zero_private_exponent(self):
        return self.has_trailing_zero(self.private_exponent)

    def trailing_zero_prime1(self):
        return self.has_trailing_zero(self.prime1)

    def trailing_zero_prime2(self):
        return self.has_trailing_zero(self.prime2)

    def trailing_zero_exponent1(self):
        return self.has_trailing_zero(self.exponent1)

    def trailing_zero_exponent2(self):
        return self.has_trailing_zero(self.exponent2)

    def trailing_zero_coefficient(self):
        return self.has_trailing_zero(self.coefficient)

    @staticmethod
    def parse_hex_line(line):
        digits = []
        trimmed_line = line.strip()
        hex_digits = trimmed_line.split(':')
        # Removes the last "" at end of line if it is required
        if hex_digits[len(hex_digits) - 1] == '':
            hex_digits = hex_digits[:len(hex_digits) - 1]
        for hex_digit in hex_digits:
            digits.append(int(hex_digit, 16))
        return digits

    def parse_block(self, lines, index):
        ret_index = index
        block = []

        line = lines[ret_index + 1]
        while line[0] == ' ':
            digits = self.parse_hex_line(line)
            if len(digits) == 0:
                ret_index = -1
                break
            else:
                block.append(digits)
                ret_index += 1
                if ret_index + 1 < self.total:
                    line = lines[ret_index + 1]
                else:
                    break

        if ret_index == -1:
            block = []
        return block

    @staticmethod
    def trim_front(component):
        work_list = component
        while work_list[0] == 0:
            work_list = work_list[1:]
        return work_list

    def parse_private_key(self, lines, index):
        ret_index = index
        line = lines[index]
        # possible syntax:
        #    RSA Private-Key: (2048 bit, 2 primes)
        #    Private-Key: (2048 bit)
        key_bits = re.findall("Private-Key: \((.*) bit", line)
        if key_bits is None:
            ret_index = -1
        else:
            if len(key_bits) == 1:
                self.key_bytes = int(int(key_bits[0]) / 8)
            else:
                ret_index = -1
        return ret_index

    def parse_modulus(self, lines, index):
        ret_index = index

        block = self.parse_block(lines, index)
        if len(block) != 0:
            count = 0
            while count < len(block):
                self.modulus = self.modulus + block[count]
                count += 1
            ret_index += len(block)
            self.modulus = self.trim_front(self.modulus)

        return ret_index

    @staticmethod
    def parse_public_exponent(index):
        # Do nothing
        return index

    def parse_private_exponent(self, lines, index):
        ret_index = index

        block = self.parse_block(lines, index)
        if len(block) != 0:
            count = 0
            while count < len(block):
                self.private_exponent = self.private_exponent + block[count]
                count += 1
            ret_index += len(block)
            self.private_exponent = self.trim_front(self.private_exponent)

        return ret_index

    def parse_prime1(self, lines, index):
        ret_index = index

        block = self.parse_block(lines, index)
        if len(block) != 0:
            count = 0
            while count < len(block):
                self.prime1 = self.prime1 + block[count]
                count += 1
            ret_index += len(block)
            self.prime1 = self.trim_front(self.prime1)

        return ret_index

    def parse_prime2(self, lines, index):
        ret_index = index

        block = self.parse_block(lines, index)
        if len(block) != 0:
            count = 0
            while count < len(block):
                self.prime2 = self.prime2 + block[count]
                count += 1
            ret_index += len(block)
            self.prime2 = self.trim_front(self.prime2)

        return ret_index

    def parse_exponent1(self, lines, index):
        ret_index = index

        block = self.parse_block(lines, index)
        if len(block) != 0:
            count = 0
            while count < len(block):
                self.exponent1 = self.exponent1 + block[count]
                count += 1
            ret_index += len(block)
            self.exponent1 = self.trim_front(self.exponent1)

        return ret_index

    def parse_exponent2(self, lines, index):
        ret_index = index

        block = self.parse_block(lines, index)
        if len(block) != 0:
            count = 0
            while count < len(block):
                self.exponent2 = self.exponent2 + block[count]
                count += 1
            ret_index += len(block)
            self.exponent2 = self.trim_front(self.exponent2)

        return ret_index

    def parse_coefficient(self, lines, index):
        ret_index = index

        block = self.parse_block(lines, index)
        if len(block) != 0:
            count = 0
            while count < len(block):
                self.coefficient = self.coefficient + block[count]
                count += 1
            ret_index += len(block)
            self.coefficient = self.trim_front(self.coefficient)

        return ret_index

    def parse(self):
        index = 0
        self.total = len(self.lines)
        lines = self.lines
        result = False

        while index < self.total:
            # possible syntax:
            #    RSA Private-Key: (2048 bit, 2 primes)
            #    Private-Key: (2048 bit)
            if re.findall('Private-Key:', lines[index]):
                index = self.parse_private_key(lines, index)
            elif re.findall('^modulus:', lines[index]):
                index = self.parse_modulus(lines, index)
            elif re.findall('^publicExponent:', lines[index]):
                index = self.parse_public_exponent(index)
            elif re.findall('^privateExponent:', lines[index]):
                index = self.parse_private_exponent(lines, index)
            elif re.findall('^prime1:', lines[index]):
                index = self.parse_prime1(lines, index)
            elif re.findall('^prime2:', lines[index]):
                index = self.parse_prime2(lines, index)
            elif re.findall('^exponent1:', lines[index]):
                index = self.parse_exponent1(lines, index)
            elif re.findall('^exponent2:', lines[index]):
                index = self.parse_exponent2(lines, index)
            elif re.findall('^coefficient:', lines[index]):
                index = self.parse_coefficient(lines, index)
            elif re.findall('^RSA key ok', lines[index]):
                result = True
                self.bad_key = False
                break
            else:
                break

            index += 1

        # Set the "short_key" flag if any of the CRT components
        # have lengths that are shorter than 1/2 of the modulus
        # size.  Example: for a 2048-bit (256 byte) modulus,
        # a parameter size of 127 would be considered short
        if self.short_prime1() or \
                self.short_prime2() or \
                self.short_exponent1() or \
                self.short_exponent2() or \
                self.short_coefficient():
            self.short_key = True

        # Set the trailing zero flag if that last field of any
        # CRT component is zero.  This information becomes
        # a factor if the openssl analysis finds the key to be invalid.
        if self.trailing_zero_prime1() or \
                self.trailing_zero_prime2() or \
                self.trailing_zero_exponent1() or \
                self.trailing_zero_exponent2() or \
                self.trailing_zero_coefficient():
            self.trailing_zero = True

        return result


#
# get_decrypt_key
# - The private key for the PKCS12 file is extracted using a set of openssl
# - The base64-enabled PKCS12 is converted to binary PKCS12 using
#        "openssl base64 -d -in $b64p12 -out $binp12"
# - The private key in PEM format is extracted from binary PKCS12 through a chained openssl command
#        "openssl pkcs12 -in $binp12 -passin pass:$pswd -passout pass:$pswd -nocerts |
#        openssl rsa -passin pass:$pswd -out $decrprivkey"
# - The content of the private key in PEM format is displayed using
#        "openssl rsa -inform PEM -text -check -noout -in $decrprivkey"
#
#
# - The method checks the status code for each subprocess.run() output
# - If at any time it does not return 0 (such as when the script is given the wrong password
# - or the pkcs12 file is misspelled), then the script will clean up the temporary directories
# - and return the last status code and the corresponding message

def get_decrypt_key(pkcs12_file, password):
    # Create temporary file for binary PKCS12 output
    try:
        pkcs12_tmp = tempfile.NamedTemporaryFile(mode='w+b')
        pkcs12_out = pkcs12_tmp.name
    except OSError:
        return 1, '\nAn error has occurred when creating the pkcs12 temporary file.\n'

    # Create temporary file for certificate
    try:
        cert_tmp = tempfile.NamedTemporaryFile(mode='w+b')
        cert_out = cert_tmp.name
    except OSError:
        return 1, '\nAn error has occurred when creating the certificate temporary file.\n'

    # Create temporary file for decrypt key
    try:
        key_tmp = tempfile.NamedTemporaryFile(mode='w+b')
        key_out = key_tmp.name
    except OSError:
        return 1, '\nAn error has occurred when creating the key temporary file.\n'

    # Generate binary PKCS12 data file
    cmd = 'openssl base64 -d -in ' + pkcs12_file + ' -out ' + pkcs12_out
    cmd = shlex.split(cmd)
    output = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if output.returncode != 0:
        return output.returncode, str(output.stderr, 'UTF-8')

    # Generate decrypt key file in PEM format
    cmd = 'openssl pkcs12 -in ' + pkcs12_out + ' -passin pass:{}'.format(shlex.quote(password))
    cmd += ' -passout pass:{}'.format(shlex.quote(password)) + ' -nocerts -out ' + cert_out
    cmd = shlex.split(cmd)
    output = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if output.returncode != 0:
        return output.returncode, str(output.stderr, 'UTF-8')
        
    cmd = 'openssl rsa -in ' + cert_out + ' -passin pass:{}'.format(shlex.quote(password)) + \
          ' -out ' + key_out
    cmd = shlex.split(cmd)
    output = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if output.returncode != 0:
        return output.returncode, str(output.stderr, 'UTF-8')

    # Decode decrypt key in PEM format
    cmd = 'openssl rsa -inform PEM -check -text -noout -in ' + key_out
    cmd = shlex.split(cmd)
    output = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #
    # MacOS returns non-0 when the key is invalid while Linux return 0 when the key both valid
    # and invalid. Needs to do a special check for MacOS (darwin) when return code is non-0.
    # In such case, concatenates the stdout and stderr to make them looks like output from Linux
    #
    if sys.platform == 'darwin' and output.returncode != 0:
        output = str(output.stdout, 'UTF-8') + str(output.stderr, 'UTF-8')
    #
    # Return error for other non-0 return code cases
    elif output.returncode != 0:
        return output.returncode, str(output.stderr, 'UTF-8')
    #
    # All looks good, get the output string
    #
    else:
        output = str(output.stdout, 'UTF-8')

    return 0, output


# main()
if __name__ == '__main__':

    # local imports
    import sys
    import argparse

    parser = argparse.ArgumentParser(description="RSA private key check")
    # Pop the optional argument group
    optional = parser._action_groups.pop()
    # Add the required argument group
    required = parser.add_argument_group('required arguments')

    required.add_argument('--pkcs12', help='pkcs12 base64 encoded file', required=True)
    optional.add_argument('--passwd', help='password for pkcs12 file', default='')
    # Append optional group after required group
    parser._action_groups.append(optional)

    # parse args
    args, sys.argv[1:] = parser.parse_known_args(sys.argv[1:])

    pkcs12 = args.pkcs12
    passwd = args.passwd

    if args.passwd == '':
        passwd = getpass.getpass(prompt='Enter password for pkcs12: ')

    if not os.path.exists(pkcs12):
        print('\nFile \'' + pkcs12 + '\' not found.\n')
        sys.exit(1)

    ret, formatted_key = get_decrypt_key(pkcs12, passwd)

    if ret != 0:
        print("\nERROR: ", formatted_key)
        sys.exit(ret)

    private_key = PrivateKey(formatted_key)
    valid = private_key.parse()

    if private_key.is_bad() and private_key.is_trailing_zero():
        print('\nThe RSA key is invalid and vulnerable to exposure due to the\n'
              'Cisco RSA Private Key Leak Vulnerability (CVE-2022-20866).\n'
              'This key should no longer be used.\n')

    elif private_key.is_bad() and not private_key.is_trailing_zero():
        print('\nThe RSA key is invalid due to the Cisco RSA Private Key Leak\n'
              'Vulnerability (CVE-2022-20866) but does not have known exposure\n'
              'characteristics. It is recommended that this key be replaced.\n')

    elif private_key.is_short():
        print('\nThe RSA key is valid but is vulnerable to exposure if used in\n'
              'product versions that are affected by the Cisco Private Key Leak\n'
              'Vulnerability (CVE-2022-20866).  If this was done, this key should\n'
              'no longer be used.\n')

    else:
        print('\nThe RSA key is valid.\n')
