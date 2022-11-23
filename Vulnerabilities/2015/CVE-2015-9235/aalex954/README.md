# JWT Key Confusion PoC (CVE-2015-9235)

Written for the Hack the Box challenges - Under Construction and Secret

Both included scripts perform a Java Web Token Key Confusion Attack (CVE-2015-9235).

To perform the attack it is required that the attacker know the public key which the server will use to verify the signature as well as the server being configured to use the HS256 algorithm. Because HS256 is a symmetric-key algorithm (the same key is used to sign and verify the message), we can use the public key to sign our tampered token. 
Since the web server knows the public key, when it receives the tampered token it will be able to verify it. 

## Screenshot

![jwt_confusion](https://user-images.githubusercontent.com/6628565/149454773-86c5f286-e411-42be-ab83-a79205ae0373.png)

## Usage


```python3 jwt-9235.py [-h] [token_location] [claim_key] [claim_value]```

```
positional arguments:
  token_location  location of JWT token (must include 'pk' payload)
  claim_key       payload claim to target
  claim_value     new claim value

optional arguments:
  -h, --help      show this help message and exit
```
If no arguments are provided the application will look for a token file ```./token```, target the ```username``` claim, and replace the claim value with ```‘ or 1=1;–```

## Requirements

```pip install hashlib hmac base64 json argparse colorama```
