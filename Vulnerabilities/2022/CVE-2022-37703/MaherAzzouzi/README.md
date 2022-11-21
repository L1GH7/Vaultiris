> [Suggested description]
> In Amanda 3.5.1, an information leak vulnerability was found in
> calcsize SUID binary. The attacker can abuse the vulnerability to know
> if a directory exist or not anywhere in the fs. The binary will use
> `opendir()` as root directly without checking the path, letting the
> attacker provide an arbitrary path. The attacker needs to be the
> `backup` user to be able to run calcsize binary.
>
> ------------------------------------------
>
> [Additional Information]
> The PoC is very simple you just have to run the binary like this:
> ./calcsize MAHER dir1 -X [directory]
> if the binary did not generate any output then the directory is available.
> If it's not available it will say that it is not available like this:
>
> backup@maher:/home/maher/pwn/ubuntu/userland/suid/amanda/sec$ ./calcsize MAHER dir1 -X /etcc
> /etcc/.: No such file or directory
>
> ------------------------------------------
>
> [Vulnerability Type]
> Insecure Permissions
>
> ------------------------------------------
>
> [Vendor of Product]
> Amanda
>
> ------------------------------------------
>
> [Affected Product Code Base]
> calcsize - 3.5.1
>
> ------------------------------------------
>
> [Affected Component]
> Component: calcsize SUID binary.
> C file: calcsize.c
> Line: 435 `if((d = opendir(dirname)) == NULL) {`
>
> ------------------------------------------
>
> [Attack Type]
> Local
>
> ------------------------------------------
>
> [Impact Information Disclosure]
> true
>
> ------------------------------------------
>
> [Attack Vectors]
> To exploit the vulnerability the attacker need to have access to the calcsize binary (one of the amanda packages being installed).
>
> ------------------------------------------
>
> [Reference]
> http://www.amanda.org/
>
> ------------------------------------------
>
> [Discoverer]
> Maher Azzouzi
