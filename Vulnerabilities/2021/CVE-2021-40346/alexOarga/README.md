# CVE-2021-40346

Integer overflow on header request internal representation allows HTTP request smuggling. This repository presents a PoC built with docker-compose using two docker images: one with a vulnerable version of HaProxy (in this case 2.2.16) and one with a Flask web server using Gunicorn as WSGI. By exploiting the vulnerability we are able to access the ```/admin``` page, whose requests are blocked by HaProxy though a user-defined rule.

# POC

## Requirements

- [Docker](https://docs.docker.com/engine/install/) :whale:
- [Docker compose](https://docs.docker.com/compose/install/) :whale:

## Environment

- HaProxy docker container with port 8000 mapped to host.
- Python docker container with Gunicorn and Flask server with port 5000 open on shared network with the HaProxy container.

## Run PoC
Run the followinf commands on terminal to bring up HaProxy docker container and Flask server image:
```bash
docker-compose up
```
Send the payload to HaProxy:
```bash
cat payload | nc localhost 8000
```

In order to bring it down run:
```bash
docker-compose down
```

## Author
[@alexOarga](https://github.com/alexOarga)



