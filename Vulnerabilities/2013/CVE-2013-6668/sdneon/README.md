CveTest
=======

Test case to replicate [Node.JS](https://github.com/joyent/node/) [V0.10.31 v8 backport CVE-2013-6668](http://blog.nodejs.org/2014/08/19/node-v0-10-31-stable/) crash.

Test calls a SQL stored procedure ("GetSomething") to retrieve data from a table ("Table1") in "SQLDB" database. Crash occurs when there is more than 52 rows returned and after making the same call several times.

Environment
-----------
* Windows 7 64-bit
* MS SQL 2005
* Node.JS V0.10.31 x64
* tedious node module V1.4.3 or earlier

Tests with various flavours of Node
-----------------------------------
* Release mode node: script just exits unexpectedly without any explicit error printed.
* Debug mode node: [assertion and stack trace](log/node_stack_trace.txt) is obtained.
* Debug mode Node with temp patch proposed in [Node.JS Issue #8208](https://github.com/joyent/node/issues/8208): still crash as above.
  * Release mode is ok.
  * The assertion encountered is in HInstruction::Verify() which is only available in DEBUG build. Thus, the alternate release build codes appear not to suffer from the CVE patch.
* Node V0.10.31 with deps/v8 from V0.10.30: No crash, works fine.
Thus, it appears to be the v8 backport CVE-2013-6668 patch that is causing the problem.

Maybe Related
-------------
* [node-mssql Issue #74](https://github.com/patriksimek/node-mssql/issues/74)
