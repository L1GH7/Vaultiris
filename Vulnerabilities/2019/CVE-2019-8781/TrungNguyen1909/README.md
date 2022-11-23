# CVE-2019-8781

## Security Advisory

[CVE-2019-8781](https://support.apple.com/en-vn/HT210634)

## POC Details

- The POC elevate the process to root and execute a shell.

## Exploit environment
	
	- macOS 10.14.6 (18G95) (Should works on all macOS version that's released before 10.15.0)
	
	- SMEP: On
	
	- SMAP: Off
	
	- Kernel ASLR slide passed to the argv[1]

## Building

- You will need Xcode 9.4.1 Command Line Tools to compile it.

- Check the `Makefile`.

## Writeups

- Checkout my blog, here is the [link](https://trungnguyen1909.github.io/blog/post/CampCTF/PwningKernelz/)

## Shoutouts

- Apple for the 0day.

- Linus Henze(@LinusHenze), for the bug, ofc =)))
