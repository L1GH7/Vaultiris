# Exploiting `dsa.Verify` in Go (CVE-2019-17596)

Please see the [associated blog post for details](https://paul.querna.org/articles/2019/10/24/dsa-verify-poc/).

# Running

Since versions of Go newer than 1.13.1 are patched, I;ve included a [Dockerfile](./Dockerfile), that makes it easier to pin your Go version.  Simply run Docker build:
```
docker build .
```

There are two files of interest:
- [`dsa_test.go`](./dsa_test.go): Contains a test case for causing `dsa.Verify` to panic/
- [`ssh_test.go`](./ssh_test.go): Contains a test case for making an `crypto/ssh.Client` to panic via an evil SSH Host Key.


## Improvements, bugs, adding feature, etc:

Please [open issues in Github](https://github.com/pquerna/poc-dsa-verify-CVE-2019-17596/issues) for ideas, bugs, and general thoughts.  Pull requests are of course preferred :)

## License

`poc-dsa-verify-CVE-2019-17596` is licensed under the [Apache License, Version 2.0](./LICENSE)
