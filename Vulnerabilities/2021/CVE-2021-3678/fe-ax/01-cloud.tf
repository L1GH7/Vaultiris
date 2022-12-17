provider "digitalocean" {
  token = var.do_token
}

module "cloud" {
  source              = "./modules/cloud"
  cloud_local_cluster = var.cloud_local_cluster
  prefix              = var.prefix
}