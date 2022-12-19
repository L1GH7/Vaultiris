# @TEST-EXEC: zeek -C -r $TRACES/gnutls-tls1.2-vulnerable.pcap $PACKAGE/cve-2020-13777.zeek %INPUT
# @TEST-EXEC: zeek-cut id.orig_h id.orig_p id.resp_h id.resp_p proto note msg sub src dst p < notice.log > notice.tmp && mv notice.tmp notice.log
# @TEST-EXEC: btest-diff notice.log


