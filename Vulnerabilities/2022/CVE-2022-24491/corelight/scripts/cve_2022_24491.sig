signature cve_2022_24491_portmap_client_set {
  ip-proto == udp
  dst-port == 111
  payload /^.*.{4}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x01/
}

signature cve_2022_24491_portmap_server_set {
  ip-proto == udp
  src-port == 111
  payload /^.{4}\x00{3}\x01/
  requires-reverse-signature cve_2022_24491_portmap_client_set
  eval CVE202224491::match_set
}

signature cve_2022_24491_portmap_client_dump {
  ip-proto == udp
  dst-port == 111
  payload /^.*.{4}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x04/
}

signature cve_2022_24491_portmap_server_dump {
  ip-proto == udp
  src-port == 111
  payload /^.{4}\x00{3}\x01/
  requires-reverse-signature cve_2022_24491_portmap_client_dump
  eval CVE202224491::match_dump
}

signature cve_2022_24491_portmap_client_tcp_set {
  ip-proto == tcp
  dst-port == 111
  payload /^.*.{8}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x01/
}

signature cve_2022_24491_portmap_server_tcp_set {
  ip-proto == tcp
  src-port == 111
  payload /^.{8}\x00{3}\x01/
  requires-reverse-signature cve_2022_24491_portmap_client_tcp_set
  eval CVE202224491::match_set
}

signature cve_2022_24491_portmap_client_tcp_dump {
  ip-proto == tcp
  dst-port == 111
  payload /^.*.{8}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x01/
}

signature cve_2022_24491_portmap_server_tcp_dump {
  ip-proto == tcp
  src-port == 111
  payload /^.{8}\x00{3}\x01/
  requires-reverse-signature cve_2022_24491_portmap_client_tcp_dump
  eval CVE202224491::match_dump
}