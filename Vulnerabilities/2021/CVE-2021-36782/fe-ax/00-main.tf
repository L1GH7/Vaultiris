terraform {
  required_providers {
    local = {
      source  = "hashicorp/local"
      version = "2.2.2"
    }
    digitalocean = {
      source  = "digitalocean/digitalocean"
      version = "~> 2.0"
    }
    rke = {
      source  = "rancher/rke"
      version = "1.3.4"
    }
    helm = {
      source  = "hashicorp/helm"
      version = "2.4.1"
    }
    rancher2 = {
      source  = "rancher/rancher2"
      version = "1.22.2"
    }
  }
}

output "rke_cluster_ips" {
  value = module.cloud.ip_addresses
}

output "rancher_admin_url" {
  value = module.rancher.admin_url
}