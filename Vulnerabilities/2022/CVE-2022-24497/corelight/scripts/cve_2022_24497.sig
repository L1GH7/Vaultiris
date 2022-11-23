signature cve_2022_24497_portmap_client {
  ip-proto == udp
  dst-port == 111
  payload /^.*.{4}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x03.*.{4}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x04/
}

signature cve_2022_24497_portmap_server {
  ip-proto == udp
  src-port == 111
  payload /^.{4}\x00{3}\x01/
  requires-reverse-signature cve_2022_24497_portmap_client
  eval CVE202224497::match_portmap
}

signature cve_2022_24497_portmap_client_tcp {
  ip-proto == tcp
  dst-port == 111
  payload /^.*.{8}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x03.*.{8}\x00{4}\x00{3}(\x01|\x02|\x03)\x00\x01\x86\xa0\x00{3}(\x01|\x02|\x03|\x04)\x00\x00\x00\x04/
}

signature cve_2022_24497_portmap_server_tcp {
  ip-proto == tcp
  src-port == 111
  payload /^.{8}\x00{3}\x01/
  requires-reverse-signature cve_2022_24497_portmap_client_tcp
  eval CVE202224497::match_portmap
}