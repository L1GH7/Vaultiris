terraform {
  required_providers {
    local = {
      source  = "hashicorp/local"
      version = "2.2.2"
    }
    rke = {
      source  = "rancher/rke"
      version = "1.3.4"
    }
  }
}

resource "rke_cluster" "cluster_local" {
  delay_on_creation  = 60
  addon_job_timeout  = 600
  enable_cri_dockerd = true
  kubernetes_version = "v1.23.10-rancher1-1"

  dynamic "nodes" {
    for_each = var.rke_cluster_ips
    content {
      address           = nodes.value
      hostname_override = "${var.prefix}-node${nodes.key + 1}"
      user              = "root"
      role              = ["controlplane", "worker", "etcd"]
      ssh_key           = var.ssh_key_pair.private_key
    }
  }

  upgrade_strategy {
    drain                  = false
    max_unavailable_worker = "20%"
  }

}

resource "local_sensitive_file" "kube_config_yaml" {
  content  = rke_cluster.cluster_local.kube_config_yaml
  filename = "kubeconfig.yaml"
}