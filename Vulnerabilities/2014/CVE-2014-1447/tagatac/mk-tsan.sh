# sudo apt-get install libdevmapper-dev libnl-dev gnulib
# sudo apt-get install libvirt0-dbg libvirt-bin
rm -rf $HOME/libvirt
rm -rf libvirt-0.9.8
[ ! -f libvirt-0.9.8.tar.gz ] && wget http://libvirt.org/sources/old/libvirt-0.9.8.tar.gz
tar xvzf libvirt-0.9.8.tar.gz
cd libvirt-0.9.8
patch -p0 < ../sleep.patch
CC=clang CFLAGS="-fsanitize=thread -g -O0" ./configure --without-udev --prefix=$HOME/libvirt 
make
make install
cd ..
$HOME/libvirt/sbin/libvirtd &
sleep 5
./run.sh
