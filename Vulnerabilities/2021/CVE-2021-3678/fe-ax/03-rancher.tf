locals {
  full_domain = join(".", [var.domain.subdomain, module.cloud.ip_addresses[0], var.domain.domain])
}

provider "rancher2" {
  alias = "bootstrap"

  api_url   = "https://${local.full_domain}"
  insecure  = true
  bootstrap = true
}

provider "rancher2" {
  alias = "admin"

  api_url   = module.rancher.admin_url
  token_key = module.rancher.admin_token
  insecure  = true
}

module "rancher" {
  source = "./modules/rancher"
  depends_on = [
    module.rke
  ]
  providers = {
    rancher2.bootstrap = rancher2.bootstrap
    rancher2.admin     = rancher2.admin
  }
  rancher_domain = local.full_domain
}

module "rancher-extra" {
  source = "./modules/rancher-extra"
  depends_on = [
    module.rke
  ]
  providers = {
    rancher2.admin = rancher2.admin
  }
  ssh_key_pair        = module.cloud.test_keypair
  cloud_extra_cluster = var.cloud_extra_cluster
  prefix              = var.prefix
}

