# Mitigate CVE-2020-8554 with Policy Controller


This repository contains configuration files for using Policy Controller, which is based on the open source OPA Gatekeeper project, to block Kubernetes Services from public IP access.

The [security advisory for this issue](https://groups.google.com/g/kubernetes-announce/c/GPpZzVtGwiI) states:
>A security issue was discovered with Kubernetes affecting multitenant clusters. If a potential attacker can already create or edit services and pods, then they may be able to intercept traffic from other pods (or nodes) in the cluster.
>
>This issue has been rated medium severity (CVSS:3.0/AV:N/AC:L/PR:L/UI:N/S:U/C:L/I:L/A:L), and assigned CVE-2020-8554.
>
>An attacker that is able to create a ClusterIP service and set the spec.externalIPs field can intercept traffic to that IP. An attacker that is able to patch the status (which is considered a privileged operation and should not typically be granted to users) of a LoadBalancer service can set the status.loadBalancer.ingress.ip to similar effect.

This repository contains a Template and Constraint that restrict Services to a specific allow list of public IPs, thus limiting the ability of an attacker to add IPs outside of trusted values.

You can apply these policies using [Policy Controller](https://cloud.google.com/anthos-config-management/docs/concepts/policy-controller), which is included as part of [Anthos Config Management](https://cloud.google.com/anthos/config-management). To customize the allowed IP addresses, edit or add items to the "allowedIPs" list in [k8sExternalIPs_constraint.yaml](https://github.com/jrmurray000/CVE-2020-8554/blob/main/k8sExternalIPs_constraint.yaml).

## Blocking by CIDR


If you just want to prevent an IP in a specific CIDR range use the files `k8sExternalIPsCIDR_constraint.yaml` and `k8sExternalIPsCIDR_template.yaml`. For example, if you want to prevent an attacker from specifying the `spec.externalIPs` field to the default Kubernetes Services CIDR.