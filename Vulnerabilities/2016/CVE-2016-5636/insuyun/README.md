# CVE-2016-5636

PoC for [CVE-2016-5636](https://bugs.python.org/issue26171), which is a heap overflow in Python 3.6. This repository
includes:
* crash-with-zip : crash PoC by reading untrusted zip file
* google-web-engine : crash PoC working on google web engine
* spawn-shell : a local exploit for spawning shell
