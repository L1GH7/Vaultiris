# the compiler to use
CC = clang

MAC_SDK = $(shell xcrun --sdk macosx --show-sdk-path)/System/Library/PrivateFrameworks/

TARGET = kcm

all: heap_leak 

heap_leak: heap_leak.c
	$(CC) heap_leak.c -F$(MAC_SDK) -framework Heimdal -o heap_leak
