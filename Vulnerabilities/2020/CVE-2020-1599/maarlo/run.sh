#!/bin/bash

sudo apt update
sudo apt install libasan5 clang clang-tools zlib1g-dev freetype2-demos gcc automake libtool -y

RUTA=$PWD

tar xvzf $RUTA/libpng-1.6.37.tar.gz
tar xvzf $RUTA/freetype2-VER-2-10-3.tar.gz

export LD_LIBRARY_PATH=/usr/local/lib/
export CFLAGS="-g -fsanitize=address -lasan" LDFLAGS="-fsanitize=address -lasan" CC=gcc

cd $RUTA/libpng-1.6.37
./configure
sudo make install

cd $RUTA/freetype2-VER-2-10-3
sh autogen.sh
./configure
sudo make
sudo make install

cd $RUTA
export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libasan.so.5

/usr/bin/ftview 150 font.ttf
