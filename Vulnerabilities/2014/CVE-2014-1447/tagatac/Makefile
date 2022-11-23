libvirt_version = 0.9.8
server_dir = $(PWD)/server
client_dir = $(PWD)/client
server_src_dir = $(server_dir)/libvirt-$(libvirt_version)
client_src_dir = $(client_dir)/libvirt-$(libvirt_version)
server_build_dir = $(server_dir)/build
client_build_dir = $(client_dir)/build
server_install_dir = $(server_dir)/install
client_install_dir = $(client_dir)/install

build : $(server_install_dir)/lib/libvirt.so $(client_install_dir)/lib/libvirt.so perftest

$(server_install_dir)/lib/libvirt.so : $(server_src_dir)/configure
	-mkdir -v $(server_build_dir)
	cd $(server_build_dir); CFLAGS="-g -O0"; \
		$(server_src_dir)/configure --without-udev --prefix=$(server_install_dir)
	make -j$(NUMCPUS) -C $(server_build_dir)
	make -j$(NUMCPUS) -C $(server_build_dir) install

$(server_src_dir)/configure : libvirt-$(libvirt_version).tar.gz sleep.patch
	make clean-server
	mkdir -v $(server_dir)
	tar xvf libvirt-$(libvirt_version).tar.gz -C $(server_dir)
	touch $(server_src_dir)/configure

$(client_install_dir)/lib/libvirt.so : $(client_src_dir)/configure
	-mkdir -v $(client_build_dir)
	cd $(client_build_dir); CFLAGS="-g -O0"; \
		$(client_src_dir)/configure --without-udev --prefix=$(client_install_dir)
	make -j$(NUMCPUS) -C $(client_build_dir)
	make -j$(NUMCPUS) -C $(client_build_dir) install

$(client_src_dir)/configure : libvirt-$(libvirt_version).tar.gz reproducer.patch
	make clean-client
	mkdir -v $(client_dir)
	tar xvf libvirt-$(libvirt_version).tar.gz -C $(client_dir)
	patch -d $(client_src_dir) -p0 < reproducer.patch
	touch $(client_src_dir)/configure

perftest : perftest.c $(server_install_dir)/lib/libvirt.so
	gcc -o perftest perftest.c -I$(server_install_dir)/include -L$(server_install_dir)/lib -lvirt

interpose.so : interpose.c
	CPATH=$(server_src_dir)/include:$(server_src_dir)/src/util:$(server_src_dir)/src/rpc:$(server_src_dir)/src:$(server_src_dir)/gnulib/lib:$(server_src_dir)/src/conf:/usr/include/libxml2:$(server_build_dir)/gnulib/lib \
		gcc -shared -ldl -fPIC -fno-builtin interpose.c -o interpose.so

.PHONY : build random test-perf clean clean-server clean-client

random :
	./gen_interpose.py $(MAX_DELAY)
	make interpose.so

test-perf : build
	LD_PRELOAD=$(PWD)/interpose.so libvirtd &
	LD_LIBRARY_PATH=$(server_install_dir)/lib ./perftest $(ITERATIONS)
	pkill libvirtd

clean : clean-server clean-client
	rm -vrf perftest interpose.c interpose.so

clean-server :
	rm -vrf $(server_dir)

clean-client :
	rm -vrf $(client_dir)
