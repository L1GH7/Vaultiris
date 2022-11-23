import base64
import hashlib
import hmac
from colorama import Fore

secret = ''

header = '{"alg":"HS256","typ":"JWT"}'
headerEncodedBytes = base64.urlsafe_b64encode(header.encode("utf-8"))

encodedHeader = str(headerEncodedBytes, "utf-8").rstrip("=")

claim = '{"name":""}'
claimEncodedBytes = base64.urlsafe_b64encode(claim.encode("utf-8"))

encodedPayload = str(claimEncodedBytes, "utf-8").rstrip("=")

newToken = (encodedHeader + "." + encodedPayload)

siginature = base64.urlsafe_b64encode(
    hmac.new(bytes(secret, "UTF-8"),newToken.encode('utf-8'),
    hashlib.sha256).digest()
    ).decode('UTF-8').rstrip("=")

newToken = newToken + "." + siginature

print(Fore.YELLOW + "NEW TOKEN: " + Fore.RESET)
print(newToken)
