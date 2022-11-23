# CVE-2021-3345

#### How to build vulnerable libgcrypt
``` 
git clone --single-branch --branch LIBGCRYPT-1.9-BRANCH https://dev.gnupg.org/source/libgcrypt.git
cd libgcrypt
git checkout aa3f595341eb
./autogen.sh
./configure --enable-maintainer-mode && make
```
#### How to use exploit
```
make
./main
```

This vulnerability is not really interesting, because most distros use an older version of libgcrypt so they are not vulnerable. There is a possibilty this can be reproduced on other software that uses libgcrypt as a dependency.
