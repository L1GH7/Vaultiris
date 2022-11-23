# CVE-2021-3572

This repository is designed for testing CVE-2021-3572 in [pypa/pip](https://github.com/pypa/pip).

For more information, see these resources:
* CVE page: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-3572
* PR where vulnerability was fixed: https://github.com/pypa/pip/pull/9827
* Issue with more discussion: https://github.com/pypa/pip/issues/10042

Also, see the tags and first two commits in this repository.

## Testing

Vulnerable version of pip (<21.1) installs version 9999.0 but the fixed version installs the correct version 1.0:

### Vulnerable version

```
$ pip install "pip<21.1"
Successfully installed pip-21.0.1

$ pip install git+https://github.com/frenzymadness/CVE-2021-3572.git@original_version

$ pip list
Package       Version
------------- -------
cve-2021-3572 9999.0
pip           21.0.1
setuptools    56.2.0
wheel         0.36.2
```

### Fixed version

```
$ pip install -U pip
Successfully installed pip-21.1.2

$ pip install git+https://github.com/frenzymadness/CVE-2021-3572.git@original_version

$ pip list
Package       Version
------------- -------
cve-2021-3572 1.0
pip           21.1.2
setuptools    56.2.0
wheel         0.36.2
```