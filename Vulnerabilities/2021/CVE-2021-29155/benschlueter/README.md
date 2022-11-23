# This is the Proof Of Concept code for CVE-2021-29155.

The range tracking system for pointer arithmetic in the speculative domain was insufficient. 

It was possible to extract kernel data via a sidechannel. 

This is a proof of concept you can read up to 0x5fff bytes out of bounds from the last element of our map onwards.

This issue was fixxed in 5.12 https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/patch/kernel/bpf/verifier.c?id=7fedb63a8307dda0ec3b8969a3b233a1dd7ea8e0 and backported to serveral LTS kernels.

However if you are still interesed how this works and see a spectre exploit in action you can run the program as sudo. Then the spectre mitigations do not kick in.

Usage:
```
sudo ./bpf_exploit 0 3 0x0 0x5ff0
```

Where 0 3 correspond to 2 different threads, which run on a different physical core for the exploit to work.

For more Information and a detailed explaination of this issue you can have a look at my bachelors thesis [NOT FINISHED RIGHT NOW]
