# @TEST-EXEC: zeek -C -r $TRACES/gnutls-tls1.2-non-vulnerable.pcap $PACKAGE/cve-2020-13777.zeek %INPUT
# @TEST-EXEC: zeek -C -r $TRACES/gnutls-tls1.3.pcap $PACKAGE/cve-2020-13777.zeek %INPUT
# @TEST-EXEC: zeek -C -r $TRACES/chrome-34-google.trace $PACKAGE/cve-2020-13777.zeek %INPUT
# @TEST-EXEC: test ! -e notice.log


