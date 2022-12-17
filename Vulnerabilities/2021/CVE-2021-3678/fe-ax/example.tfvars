prefix = "cve-2021-36782"

do_token = ""

domain = {
  domain    = "sslip.io"
  subdomain = "rancher"
}

cloud_local_cluster = {
  count  = 3
  image  = "ubuntu-20-04-x64"
  region = "ams3"
  size   = "s-2vcpu-4gb-intel"
}

cloud_extra_cluster = {
  count  = 3
  image  = "ubuntu-20-04-x64"
  region = "ams3"
  size   = "s-2vcpu-4gb-intel"
}

