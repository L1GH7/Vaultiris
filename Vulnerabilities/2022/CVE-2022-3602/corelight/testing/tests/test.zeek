# @TEST-DOC: Test for creation of exploit notices
# @TEST-DOC: pcaps are from https://github.com/fox-it/spookyssl-pcaps
# @TEST-EXEC: zeek -Cr $TRACES/spookyssl-merged.pcap $PACKAGE %INPUT
# @TEST-EXEC: cat notice.log | zeek-cut id.orig_h id.orig_p id.resp_h id.resp_p note msg sub > notice_cut_exploit.log
# @TEST-EXEC: btest-diff notice_cut_exploit.log

# @TEST-DOC: Test for creation of vulnerable version notices.
# @TEST-EXEC: zeek -Cr $TRACES/sample_OpenSSLv3.0.5.pcap $PACKAGE %INPUT
# @TEST-EXEC: cat notice.log | zeek-cut id.orig_h id.orig_p id.resp_h id.resp_p note msg sub > notice_cut_vulnerable.log
# @TEST-EXEC: btest-diff notice_cut_vulnerable.log
