# folder where gcrypt.h is
INC = libgcrypt/src
# library to link to
LNK = libgcrypt/src/.libs/libgcrypt.so.20.3.0
# folder to libraries
LBS = libgcrypt/src/.libs

CC = clang
LDFLAGS = -I $(INC) $(LNK) -lgpg-error -Wl,-rpath,$(LBS) # -fsanitize=address -fno-omit-frame-pointer
CXXFLAGS = -Wall -Wextra -g

BINS = main

all: $(BINS)
	$(CC) $(CXXFLAGS) main.c -o main $(LDFLAGS)
