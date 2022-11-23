ifdef SSL_BASEDIR
FLAGS=-I$(SSL_BASEDIR)/include -L$(SSL_BASEDIR)/lib -Wl,-rpath=$(SSL_BASEDIR)/lib
endif

all: cve2018_8970_demo
	./$<
	@echo ""
	@echo "CVE2018-8970: Expected a hostname mismatch error"

.PHONY: cve2018_8970_demo
cve2018_8970_demo: cve2018_8970_demo.c
	cc $(FLAGS) -lcrypto -lssl $< -o $@
