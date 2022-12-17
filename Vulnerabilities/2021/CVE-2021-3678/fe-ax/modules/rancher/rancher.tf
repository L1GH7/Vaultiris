resource "helm_release" "rancher" {
  name       = "rancher"
  namespace  = "cattle-system"
  chart      = "rancher"
  repository = "https://releases.rancher.com/server-charts/stable"
  depends_on = [helm_release.cert_manager]
  version    = "2.6.6"

  wait             = true
  wait_for_jobs    = true
  create_namespace = true
  force_update     = true
  replace          = true

  set {
    name  = "hostname"
    value = var.rancher_domain
  }

  set {
    name  = "ingress.tls.source"
    value = "rancher"
  }

  set {
    name  = "bootstrapPassword"
    value = "A-Random-Password"
  }

  set {
    name  = "replicas"
    value = "1"
  }
}

# Create a new rancher2_bootstrap using bootstrap provider config
resource "rancher2_bootstrap" "admin" {
  provider         = rancher2.bootstrap
  depends_on       = [helm_release.rancher]
  initial_password = "A-Random-Password"
  # New password will be generated and saved in statefile
  telemetry = true

}

resource "local_sensitive_file" "rancher_password" {
  content  = join("", [rancher2_bootstrap.admin.password, "\n"])
  filename = "rancher_password"
}

