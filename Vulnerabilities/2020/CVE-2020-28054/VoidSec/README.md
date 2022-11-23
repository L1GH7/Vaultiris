# Tivoli-Madness
Advisory for:

+ CVE-2020-28054: An Authorization Bypass vulnerability affecting JamoDat – TSMManager Collector v. <= 6.5.0.21
+ A Stack Based Buffer Overflow affecting IBM Tivoli Storage Manager (Command Line Administrative Interface) Version 5, Release 2, Level 0.1. 

	Unfortunately, after I had one of the rudest encounters with an Hackerone’s triager, these are the takeaways: 
	+ IBM Tivoli Storage Manager has reached its end of life support and will not be patched.
	+ No CVE number was released.
	+ I cannot verify if this vulnerability is also affecting the newer IBM Spectrum Protect, so, good luck with that.

### You can read more on: [https://voidsec.com/tivoli-madness](https://voidsec.com/tivoli-madness)