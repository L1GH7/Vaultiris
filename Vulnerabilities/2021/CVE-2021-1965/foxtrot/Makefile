CC=clang
CFLAGS=-Wall -Werror -O3 -o poc
LDFLAGS=-lpcap -lwifi

poc: poc.o
	$(CC) $(CFLAGS) poc.c $(LDFLAGS) -ggdb -g

clean:
	rm poc *.o
