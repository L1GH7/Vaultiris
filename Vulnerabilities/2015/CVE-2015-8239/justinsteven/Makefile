.PHONY: clean image
all: hello goodbye image

hello: hello.c
	gcc -o $@ $<

goodbye: goodbye.c
	gcc -o $@ $<

image:
	sudo docker build --tag=justinsteven/sudo_digest_race .

clean:
	rm -f hello goodbye
	sudo docker rmi justinsteven/sudo_digest_race
