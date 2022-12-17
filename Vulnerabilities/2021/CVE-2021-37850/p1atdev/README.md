# CVE-2021-37850 PoC (旧 eset-killer)

ESET を停止させます。
日本語環境のみ対応。

## 使用されている脆弱性

CVE-2021-37850
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-37850

## 使い方

以下のように実行します。

```bash
$ python main.py
```

すると改造済み ESET が起動するので、設定からプレゼンテーションモードを有効にし、ホームからリスタートできます。

または、

```bash
$ python main.py ~/Downloads/eset
```

とすると、`~/Downloads/eset`内に ESET がコピーされます。
指定しない場合は`/tmp`内に作成されます。
