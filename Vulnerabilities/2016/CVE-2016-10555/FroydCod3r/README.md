# Change the algorithm RS256(asymmetric) to HS256(symmetric) - POC (CVE-2016-10555)

The algorithm HS256 uses the secret key to sign and verify each message.
The algorithm RS256 uses the private key to sign the message and uses the public key for authentication.

If you change the algorithm from RS256 to HS256, the back end code uses the public key as the secret key and then uses the HS256 algorithm to verify the signature.


Regards:
https://www.youtube.com/c/FarahHawa
