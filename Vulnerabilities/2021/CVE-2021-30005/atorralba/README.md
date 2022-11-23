# CVE-2021-30005-POC

PoC for [CVE-2021-30005](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-30005).

## Details

The vulnerability resides in the fact that PyCharm would automatically activate a virtual environment found in the project when opened for the first time. This allowed an attacker to create a repository containing a malicious virtual environment with arbitrary commands in the activation scripts (e.g. `venv/bin/activate`), that would get executed when downloaded and opened in PyCharm.

The injected command can be found [here](https://github.com/atorralba/CVE-2021-30005-POC/blob/2f5a4d99bac731200718883a8faa008b08788c88/venv/bin/activate#L4).

## To reproduce

Clone the repo:

```
git clone https://github.com/atorralba/CVE-2021-30005-POC
```
And open it in a vulnerable version of PyCharm (before 2020.3.4).

## More information about the fix

See **Trusted Projects** in https://blog.jetbrains.com/pycharm/2021/03/pycharm-2020-3-4-is-out/

## References

* Inspired by: https://blog.doyensec.com/2020/03/16/vscode_codeexec.html
* JetBrains security bulletin: https://blog.jetbrains.com/blog/2021/05/07/jetbrains-security-bulletin-q1-2021/
