resource "rancher2_cluster" "extra_cluster" {
  provider    = rancher2.admin
  name        = "extra-cluster"
  description = "Terraform extra-cluster"

  rke_config {
    enable_cri_dockerd = true
    kubernetes_version = "v1.23.10-rancher1-1"
    network {
      plugin = "canal"
    }
    upgrade_strategy {
      drain                  = false
      max_unavailable_worker = "20%"
    }
  }
}

# Create a new rancher2 Cluster Sync
resource "rancher2_cluster_sync" "extra_cluster" {
  provider = rancher2.admin

  cluster_id    = rancher2_cluster.extra_cluster.id
  state_confirm = 3
}

resource "rancher2_project" "extraproject" {
  provider = rancher2.admin
  depends_on = [
    rancher2_cluster_sync.extra_cluster
  ]

  name       = "extraproject"
  cluster_id = rancher2_cluster.extra_cluster.id
}

resource "rancher2_namespace" "foo" {
  provider    = rancher2.admin
  name        = "foo"
  project_id  = rancher2_project.extraproject.id
  description = "foo namespace"
}

resource "rancher2_user" "readonlyuser" {
  provider = rancher2.admin

  name     = "readonlyuser"
  username = "readonlyuser"
  password = "readonlyuserreadonlyuser"
  enabled  = true
}

resource "rancher2_global_role_binding" "readonlyuser" {
  provider = rancher2.admin
  depends_on = [
    rancher2_project.extraproject,
    rancher2_user.readonlyuser
  ]

  name           = "readonlyuser"
  global_role_id = "user-base"
  user_id        = rancher2_user.readonlyuser.id
}

resource "rancher2_project_role_template_binding" "readonlyuser-binding" {
  provider = rancher2.admin
  depends_on = [
    rancher2_cluster_sync.extra_cluster
  ]

  name             = "readonlyuser-binding"
  project_id       = rancher2_project.extraproject.id
  role_template_id = "workloads-view"
  user_id          = rancher2_user.readonlyuser.id
}