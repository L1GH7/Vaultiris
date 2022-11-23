## Module: Phoenix Talon CVE 2017-8890

This may resolve issue [#8571](https://github.com/rapid7/metasploit-framework/issues/8571), which requests Phoenix Talon modules.

## Overview of 2017-8890

[This CVE:](https://nvd.nist.gov/vuln/detail/CVE-2017-8890)
- is the most serious member of the Phoenix Talon class of Linux kernel vulnerabilities. No POC of this CVE exists in the [Exploit DB](https://www.exploit-db.com/search?verified=true&hasapp=true&nomsf=true). But other public POC's are available.
- is not very well-documented. But here's a [short explanation](https://2freeman.github.io/2018/01/06/CVE-2017-8890-internals.html) to give you a general understanding of the vulnerability.
- exists in all kernel versions through 4.10.15, although some sources say it exists through 4.11. [See the patch commit here](https://github.com/torvalds/linux/commit/657831ffc38e30092a2d5f03d385d710eb88b09a).
- is the result of a flaw in the kernel's IPv4 stack (specifically, multicast).

On the target machine, a double-free is triggered due to the kernel keeping an extra copy of `mc_list` at `accept()` time.

#### Pseudocode with explanations:

A machine running a kernel 4.10.15 and under is at risk if it is running the following routine:
```
sockfd = socket(AF_INET, xx, IPPROTO_TCP);
setsockopt(sockfd, SOL_IP, MCAST_JOIN_GROUP, xxxx, xxxx);
bind(sockfd, xxxx, xxxx);
listen(sockfd, xxxx);
newsockfd = accept(sockfd, xxxx, xxxx);
close(newsockfd);  // trigger release calls, handoff to RCU
sleep(5);          // wait for rcu to free()
close(sockfd);     // second free()
```

The parent socket, `sockfd`, is created. It is added to the multicast group with option MCAST_JOIN_GROUP.
In adding the socket to the multicast group on the local interface, the kernel allocates memory. At this point,
`mc_list` exists in the parent socket.

After an address is assigned to the socket with `bind()`, `listen()` for the connection and `accept()`.
`accept()` creates a new socket, `newsockfd`, to which all necessary fields of the parent are copied, 
including the value of the `mc_list` pointer. At this point, there are multiple pointers pointing
to the same block of memory, hence the double free.

When the connection is established, the kernel creates a child socket that inherits the `mc_list` object of the parent socket.
This inheritence flaw is in the `inet_csk_clone_lock` in line 648 of file [net/ipv4/inet_connection_sock.c](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/net/ipv4/inet_connection_sock.c?h=v4.9-rc5).
View [the patch](https://github.com/torvalds/linux/commit/657831ffc38e30092a2d5f03d385d710eb88b09a) to see the one-line fix for this unintended inheritence.

Next, close the child socket. As explained above, this does not release the `mc_list` object.
It goes through the RCU (remote-copy-update) structure to release memory.
`sleep()` for a few seconds to make sure the RCU handoff has enough time to call `kfree()`.
Finally, close the parent socket, which will trigger the second free.

## The Module
Simple DoS. Remotely trigger double-free on known target machine running the requesite server routine (explained above). This causes a kernel panic.
 
