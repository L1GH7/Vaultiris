# cve-2016-5699-report
Reading Course Report

This repo references an [example](https://github.com/bunseokbot/CVE-2016-5699-poc).

`simple-client.py` is a Python script to make HTTP requests to the URL passed from a command argument.  
`simple-server.py` uses Flask to construct a simple HTTP server to print the headers of received requests.

## Set Up
* Install virtualenv, `pip install virtualenv`
* Get two versions of Python for comparison, one before 2.7.10, the other after 2.7.10. This [link](http://mbless.de/blog/2016/01/09/upgrade-to-python-2711-on-ubuntu-1404-lts.html) might be useful.
* Create two Python environments with `virtualenv`. For example:
  * `virtualenv -p [path_to_one_Python_version] venv-one-version`
  * `virtualenv -p [path_to_the_other_Python_version] venv-the-other-version`
* Install `flask` in both environments. Activate corresponding environment, and `pip install flask`

## Run
* Choose the version you want to use, activate using `source ./venv-one-version/bin/activate`
* To start the server, simply run `python simple-server.py`
* To run the client:
  * run `python simple-client.py http://127.0.0.1:8000/test-url` for healthy URL
  * run `python simple-client.py http://127.0.0.1%0d%0aX-injected:%20header%0d%0ax-leftover:%20:8000/test-url` for malicious URL

## References:
* http://blog.blindspotsecurity.com/2016/06/advisory-http-header-injection-in.html
* https://github.com/bunseokbot/CVE-2016-5699-poc
* https://hg.python.org/cpython/rev/1c45047c5102
