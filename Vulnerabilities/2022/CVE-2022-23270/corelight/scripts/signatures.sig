signature cve_2022_23270 {
  ip-proto == tcp
  dst-port == 1723
  # https://datatracker.ietf.org/doc/html/rfc2637#section-2.5
  payload /(\x00\x10\x00\x01\x1a\x2b\x3c\x4d\x00\x05\x00\x00....){20}/
  payload-size > 1300
  tcp-state originator
}

signature cve_2022_23270_2 {
  ip-proto == tcp
  src-port == 1723
  tcp-state responder
  payload /^$/
  requires-reverse-signature cve_2022_23270
  eval CVE202223270::match
}