# bob_kern_exp1
CVE-2022-1015


### requirements
```bash
apt install libmnl-dev libnftnl-dev
```

### build
```bash
gcc -o leak leak.c -lmnl -lnftnl
```
