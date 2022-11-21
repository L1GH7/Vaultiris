# CVE-2022-31629 poc

## [PHP Bug report](https://bugs.php.net/bug.php?id=81727)

## How to test

### Install

```shell
git clone https://github.com/SilNex/CVE-2022-31629-poc
cd ./CVE-2022-31629-poc
docker-compose up -d
```

### TEST

`https://localhost:8110` : v8.1.10  
`https://localhost:8111` : v8.1.11

### Chrome HSTS issue

`thisisunsafe` 를 hsts페이지에서 입력하면됩니다.

Typing `thisisunsafe` on hsts error page.