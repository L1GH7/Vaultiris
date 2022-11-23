signature cve_2022_26937_NLM {
  # NFS can be on any port
  ip-proto == udp
  # RPC header
  payload /^.{4}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xb5\x00{3}(\x01|\x02|\x03|\x04)/
  tcp-state originator
  eval CVE202226937::match_nlm
}

signature cve_2022_26937_getaddr {
  ip-proto == udp
  dst-port == 111
  # RPC header
  payload /^.{4}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00{3}\x03/
  tcp-state originator
}

signature cve_2022_26937_getaddr_2 {
  ip-proto == udp
  src-port == 111
  tcp-state responder
  # RPC header
  payload /^.{4}\x00{3}\x01\x00{4}/
  payload-size > 150
  requires-reverse-signature cve_2022_26937_getaddr
  eval CVE202226937::match_getaddr
}

signature cve_2022_26937_NLM_tcp {
  # NFS can be on any port
  ip-proto == tcp
  # RPC header
  payload /^.{8}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xb5\x00{3}(\x01|\x02|\x03|\x04)/
  tcp-state originator
  eval CVE202226937::match_nlm
}

signature cve_2022_26937_tcp_getaddr {
  ip-proto == tcp
  dst-port == 111
  # RPC header
  payload /^.{8}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00{3}\x03/
  tcp-state originator
}

signature cve_2022_26937_tcp_getaddr_2 {
  ip-proto == tcp
  src-port == 111
  tcp-state responder
  # RPC header
  payload /^.{8}\x00{3}\x01\x00{4}/
  payload-size > 150
  requires-reverse-signature cve_2022_26937_tcp_getaddr
  eval CVE202226937::match_getaddr
}