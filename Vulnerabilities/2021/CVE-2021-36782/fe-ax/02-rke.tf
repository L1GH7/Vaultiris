provider "helm" {
  kubernetes {
    config_path = "kubeconfig.yaml"
  }
}

provider "rke" {
  debug    = true
  log_file = "rke_debug.log"
}

module "rke" {
  source = "./modules/rke"
  depends_on = [
    module.cloud
  ]
  domain          = var.domain
  ssh_key_pair    = module.cloud.test_keypair
  rke_cluster_ips = module.cloud.ip_addresses
  prefix          = var.prefix
}