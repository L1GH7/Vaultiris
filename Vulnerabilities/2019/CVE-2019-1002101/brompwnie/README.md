# kubectl_cp_CVE-2019-1002101
PoC helper scripts and Dockerfile for CVE-2019-1002101

For detailed info, goto https://www.twistlock.com/labs-blog/disclosing-directory-traversal-vulnerability-kubernetes-copy-cve-2019-1002101/ and https://discuss.kubernetes.io/t/announce-security-release-of-kubernetes-kubectl-potential-directory-traversal-releases-1-11-9-1-12-7-1-13-5-and-1-14-0-cve-2019-1002101/5712

## Usage
These steps will help you get a shell via one technique, there are many others.
- Step 1, on your target host, execute setupTar.sh to get your evul tar file and make sure badbin is available
- Step 2, prep the target host to execute your evul "tar" binary by running setupTar.sh (you need perms to write to /bin/tar)
- Step 3, Wait for target to run ```kubectl cp``` and a new bash session to be started

# Extras
- 'Dockerfile' can be used to replace all available path commands to execute your malicious binary, phun to test if a system is executing binaries in user supplied containers.
- replace.sh is a script which can be used to replace all binaries in the current path and ensure that any calls to /bin/abinary instead calls your binary i.e baddbin
