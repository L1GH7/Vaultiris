terraform {
  required_providers {
    local = {
      source  = "hashicorp/local"
      version = "2.2.2"
    }
    rancher2 = {
      source                = "rancher/rancher2"
      version               = "1.22.2"
      configuration_aliases = [rancher2.bootstrap, rancher2.admin]
    }
    helm = {
      source  = "hashicorp/helm"
      version = "2.4.1"
    }
  }
}

output "admin_url" {
  value = rancher2_bootstrap.admin.url
}

output "admin_token" {
  value = rancher2_bootstrap.admin.token
}