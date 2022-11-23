CoreGraphics CCITT Memory Corruption - CVE-2014-4481
====================================================

Apple CoreGraphics framework fails to validate the input when parsing CCITT group 3 encoded data resulting in a heap overflow condition. A small heap memory allocation can be overflowed with controlled data from the input resulting in arbitrary code execution in the context of Mobile Safari

Summary
========
* Title: Apple CoreGraphics Memory Corruption
* CVE Name: CVE-2014-4481
* Permalink: http://blog.binamuse.com/2015/01/coregraphics-ccitt-memory-corruption.html
* Date published: 2015-01-27
* Date of last update: 2015-01-27
* Class: Client side / Integer Overflow / Memory Corruption
* Advisory: HT204245

