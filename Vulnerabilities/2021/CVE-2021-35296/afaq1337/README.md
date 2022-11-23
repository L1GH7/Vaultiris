# CVE-2021-35296
# Author: ``M. Afaq Abid``

PoC of CVE-2021-35296 - FiberHome VDSL2 Modem HG150-Ub_V3.0 (PTCL)

## Authentication Bypass through response manipulation.


## Steps to Reproduce:

1. Go to login page of Modem (192.168.10.1)
2. Type any random Username and Password and capture the request in any proxy listener. (BurpSuite)
3. Forward the request with above random Username and Password with the option of Intercept Response.
4. When the response returns, update ``Set-Cookie: Name=;`` with ``Set-Cookie: Name=0admin;`` and in body tag modify the ``parent.location='login.html'`` with ``parent.location='/'``
5. Forward the request and you are in the Admin Panel without any credentials.

PoC video for more details: https://youtu.be/kNdAIGcNvXU
