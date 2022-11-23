# Generative testing bouncy castle vulnerability (CVE-2020-28052)

## Running the tests

Test the vulnerable versions (defaults to 1.66)
```
$ clojure -M:test
$ clojure -M:test:v1.65
```

Test the versions before the vuln and after the patch (1.67)

```
$ clojure -M:test:v1.64
$ clojure -M:test:patched
```


## Links

https://www.synopsys.com/blogs/software-security/cve-2020-28052-bouncy-castle/

https://www.bleepingcomputer.com/news/security/bouncy-castle-crypto-authentication-bypass-vulnerability-revealed/

https://twitter.com/saleemrash1d/status/1339998265899687937

https://github.com/bcgit/bc-java/issues/627

https://github.com/bcgit/bc-java/commit/00dfe74aeb4f6300dd56b34b5e6986ce6658617e?branch=00dfe74aeb4f6300dd56b34b5e6986ce6658617e&diff=split

https://github.com/bcgit/bc-java/commit/97578f9b7ed277e6ecb58834e85e3d18385a4219#diff-60a5e3ff66dd8fb8000244e246c0e2d6db9c2b8e2baedd7fda9eb5a215dee28f
