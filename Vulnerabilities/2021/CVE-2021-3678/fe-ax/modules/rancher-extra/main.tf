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
    rancher2 = {
      source                = "rancher/rancher2"
      version               = "1.22.2"
      configuration_aliases = [rancher2.admin]
    }
  }
}

resource "digitalocean_droplet" "extra_nodes" {
  count    = var.cloud_extra_cluster.count
  name     = "${var.prefix}-extra-node${count.index + 1}"
  image    = var.cloud_extra_cluster.image
  region   = var.cloud_extra_cluster.region
  size     = var.cloud_extra_cluster.size
  ssh_keys = var.ssh_key_pair.do_fingerprint
  connection {
    type        = "ssh"
    user        = "root"
    private_key = var.ssh_key_pair.private_key
    host        = self.ipv4_address
  }
  provisioner "file" {
    destination = "/tmp/provision.sh"
    content = templatefile(
      "${path.module}/provision-docker-extra.tftpl", {
        node_command = rancher2_cluster.extra_cluster.cluster_registration_token[0].node_command,
        worker       = true,
        etcd         = true,
        controlplane = true
      }
    )
  }
  provisioner "remote-exec" {
    inline = ["/bin/bash /tmp/provision.sh"]
  }
}

output "ip_addresses_extra" {
  value = digitalocean_droplet.extra_nodes[*].ipv4_address
}