build:
	docker build -t cve-server -f Dockerfile .

start:
	docker run -p 8080:80 --rm -it cve-server

build-patched:
	docker build -t cve-server-patched -f Dockerfile-patched .

start-patched:
	docker run -p 8080:80 --rm -it cve-server-patched
