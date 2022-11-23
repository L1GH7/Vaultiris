# Martian Packets

A repo containing a python script which can be used to craft raw TCP/IP packets.

Used for testing out martian packets following a vulnerability announced in kubernetes (kube-proxy: CVE-2020-8558). This PoC covers the pod -> node (host) localhost boundary bypass.

For a simple and robust PoC for the node -> node portion of the vulnerability, see [here](https://github.com/kubernetes/kubernetes/issues/90259).

Packet crafting credits:  
https://inc0x0.com/tcp-ip-packets-introduction/tcp-ip-packets-3-manually-create-and-send-raw-tcp-ip-packets/  
https://gist.github.com/NickKaramoff/b06520e3cb458ac7264cab1c51fa33d6  

## Kube

There is a Dockerfile and pod manifest for deploying the exploit to kubernetes for easily proving whether a cluster is vulnerable. The script has been updated to target the unauthenticated kube api server on port 8080 which runs on master nodes (the manifest will deploy the pod to a master node).

The exploit will use the kube apiserver's REST API to create a pod in your cluster's default namespace with the name: `youve-been-pwned`. This pod just echos out text in an infinite loop and will need deleting manually.  

I had some issues on certain clusters (running cilium) when starting the process immediately on container creation, where no syn/ack would be received from the host after sending an initial syn. I've hacked around this with the [start.sh](./start.sh) script, which just checks that the network is up before starting the exploit - this seemed to help.
