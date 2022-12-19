"""
Author: Sander Ubink
Date: 20-04-2020
Description: Decryptor for passwords stored in a Jobscheduler (S)FTP profile configuration file. The password is encrypted using the name of the profile as the key. E.g. if the profile starts with '[profile example]', the key is 'example'.
"""

import pyDes
import base64
import argparse

parser = argparse.ArgumentParser(description="Decrypt the password stored in a Jobscheduler (S)FTP profile configuration file")
parser.add_argument("password", help="password to be decrypted")
parser.add_argument("profilename", help="name of the profile")
args = parser.parse_args()

if len(args.profilename) > 24:
	sys.exit("Profile name is longer than 24 characters. Check the validity of the input.")

key = args.profilename + ((24 - len(args.profilename)) * " ")
cipher = pyDes.triple_des(key, pyDes.ECB, b"\0\0\0\0\0\0\0\0", pad=" ", padmode=None)
plain = cipher.decrypt(base64.b64decode(args.password))

print(plain)