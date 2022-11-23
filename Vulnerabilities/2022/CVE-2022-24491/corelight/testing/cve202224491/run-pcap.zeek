# @TEST-DOC: Example of a test that runs Zeek on a pcap and verifies log content
# @TEST-EXEC: zeek -Cr $TRACES/CVE-2022-24491.pcap $PACKAGE %INPUT >output
# @TEST-EXEC: btest-diff output
# @TEST-EXEC: btest-diff conn.log
# @TEST-EXEC: btest-diff notice.log
