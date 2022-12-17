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
    tls = {
      source  = "hashicorp/tls"
      version = "4.0.4"
    }
  }
}

resource "digitalocean_project" "rancher_test_setup" {
  name        = "Rancher test setup"
  description = "The local nodes were rancher runs"
  purpose     = "Rancher test nodes"
  environment = "Development"
  resources   = digitalocean_droplet.local_nodes[*].urn
}

# Create a keypair specific for this test environment

resource "tls_private_key" "test_keypair" {
  algorithm = "RSA"
  rsa_bits  = 4096
}

# Write the openssh keyfiles down

resource "local_sensitive_file" "test_keypair_privatekey" {
  content  = tls_private_key.test_keypair.private_key_openssh
  filename = "test_keypair"
}

resource "local_sensitive_file" "test_keypair_publickey" {
  content  = tls_private_key.test_keypair.public_key_openssh
  filename = "test_keypair.pub"
}

resource "digitalocean_ssh_key" "test_keypair" {
  name       = "test_keypair"
  public_key = tls_private_key.test_keypair.public_key_openssh
}

resource "digitalocean_droplet" "local_nodes" {
  count    = var.cloud_local_cluster.count
  name     = "${var.prefix}-local-node${count.index + 1}"
  image    = var.cloud_local_cluster.image
  region   = var.cloud_local_cluster.region
  size     = var.cloud_local_cluster.size
  ssh_keys = [digitalocean_ssh_key.test_keypair.fingerprint]
  connection {
    type        = "ssh"
    user        = "root"
    private_key = tls_private_key.test_keypair.private_key_openssh
    host        = self.ipv4_address
  }

  provisioner "remote-exec" {
    script = "${path.module}/provision-docker.tftpl"
  }
}

output "ip_addresses" {
  value = digitalocean_droplet.local_nodes[*].ipv4_address
}

output "test_keypair" {
  value = {
    private_key    = tls_private_key.test_keypair.private_key_openssh
    public_key     = tls_private_key.test_keypair.public_key_openssh
    do_fingerprint = [digitalocean_ssh_key.test_keypair.fingerprint]
  }
}