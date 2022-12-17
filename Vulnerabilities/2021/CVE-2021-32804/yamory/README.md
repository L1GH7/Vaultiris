# CVE-2021-32804

yamory blog 「[CVE-2021-32804 npmにも影響があるnode-tarのパストラバーサルの脆弱性](https://yamory.io/blog/cve-2021-32804-node-tar-reproduce/)」 で利用したDocker環境の構築を行うためのレポジトリです。

## 環境構築

```
docker build -t cve-2021-32804 .
docker run -it cve-2021-32804 /bin/bash
```

## 脆弱性の再現

### node-tar

```
root@5e1af2148215:/poc# node tar.js
root@5e1af2148215:/poc# su node
Hello, world!
```

### npm

```
root@5e1af2148215:/poc# npm i bashrc.tgz
root@5e1af2148215:/poc# su node
Hello, world!
```

