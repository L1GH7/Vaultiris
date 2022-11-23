# Prisma Cloud Compute Mitigations for Kubernetes CVE-2020-8554
This repository contains Prisma Cloud Compute Admission rules that mitigate exploitation of CVE-2020-8554, an unpatched Kubernetes vulnerability. To ensure correct usage, please follow the instructions provided in the 'Prisma Cloud Mitigation' section of our response post, [Protecting Against Kubernetes CVE-2020-8554](https://unit42.paloaltonetworks.com/).

## Kubernetes CVE-2020-8554
Kubernetes CVE-2020-8554 is a design flaw allowing services to intercept cluster traffic to certain IPs, by setting their `.spec.exteralIPs` or `.status.loadBalancer.ingress[].ip` fields to those IPs. Non-admin users that can create or update services, or patch services' status, can exploit the vulnerability to perform [Man-in-The-Middle](https://attack.mitre.org/techniques/T1557/) attacks against pods and nodes in the cluster in an attempt to escalate their privileges.

## Further Reading 
For more information, refer to [Protecting Against Kubernetes CVE-2020-8554](https://unit42.paloaltonetworks.com/).
