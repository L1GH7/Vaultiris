# PoC TLS1.3 CVE-2020-13777

## The purpose of this PoC

This PoC and related article were created to apply for the project "[Challenge CVE-2020-13777](https://jovi0608.hatenablog.com/entry/2020/06/13/104905)".
[CVE-2020-13777](https://gnutls.org/security-new.html#GNUTLS-SA-2020-06-03) is a GnuTLS vulnerability whose patch is widely distributed.
This PoC aims to contribute to improving the information security literacy of people involved in the information and communications industry.
By widely disclosing the PoC and technical verification results, we hope that this vulnerability will be publicly recognized by those involved in the information and communications industry.

## このPoCの目的

関連記事及び、企画「[Challenge CVE-2020-13777](https://jovi0608.hatenablog.com/entry/2020/06/13/104905)」へ提供した解説文章・PoCは情報通信産業に関わる人の情報セキュリティのリテラシーの向上に貢献することを目的としています。
修正パッチが広く配布されている脆弱性である[CVE-2020-13777](https://gnutls.org/security-new.html#GNUTLS-SA-2020-06-03)について、修正パッチ配布後に開発元によって公開された脆弱性の情報を元に技術的な検証を行った結果を啓蒙活動の一環として公開しています。
それにより、この脆弱性について情報通信産業に関わる人に広く認知されることを期待しています。

## Disclaimers

This PoC is not permitted for any purpose other than academic or technical verification or education.  
Do not run PoC except in the following environment.

- The computer permitted to use PoC by the owner.

This PoC provides under MIT LICENSE.  
The authors don't take responsibility for any damage caused by using this program.

This PoC was created to understand TLS 1.3 reconnect ability and apply for Challenge CVE-2020-13777.
Check https://jovi0608.hatenablog.com/entry/2020/06/13/104905 for Challenge CVE-2020-13777(Japanese).

This PoC was created for technical verification of CVE-2020-13777 in offline environment.
It does not have the ability to attack a real server with CVE-2020-13777, and is not intended to be an actual attack.
This PoC was designed to parse pcap files that the organizer of Challenge CVE-2020-13777 has allowed to distribute and parse. It works offline without any communication with external servers.

## 免責事項

このPoCを、学術的または技術的な検証または教育以外の目的で利用しないでください。  
また、以下の環境以外でPoCを実行しないでください。  

- コンピュータの所有者によってPoCの実行を許可されたコンピュータ

また、プログラムはMITライセンスで提供されています。 作者は利用者が本プログラムによって被った損害、損失に対して、いかなる場合でも一切の責任を負いません。

このPoCおよび解説文は、TLS 1.3の再接続性を理解し、Challenge CVE-2020-13777に応募するために作成されました。Challenge CVE-2020-13777については、https://jovi0608.hatenablog.com/entry/2020/06/13/104905 を確認してください。

なお、作成し公開したPoCはオフラインでの技術的な検証を目的に作成されたもので、実在のサーバーに対してCVE-2020-13777を用いた攻撃を行う能力は無く、また攻撃を意図して作成したものではありません。
このPoCは主催者が配布・解析を許可したパケット情報をまとめたファイル(pcapファイル)を解析するように設計しており、外部のサーバーとの通信は一切行わずオフラインで動作します。

## Get Started

- Requirements
  - Python3.6.x later
  - Following python packages
    - scapy, cryptography, pycryptodome, hashlib

git clone & install packages
```bash
git clone --recurse git@github.com:prprhyt/PoC_TLS1_3_CVE-2020-13777.git
pip3 install -r requirements.txt
```

Run PoC
```bash
python3 main.py
b"Let's study TLS with Professional SSL/TLS!\n\n\x17"
4c6574277320737475647920544c5320776974682050726f66657373696f6e616c2053534c2f544c53210a0a17
```

## Related articles

- About Challenge CVE-2020-13777(Japanese):
  - 求む！TLS1.3の再接続を完全に理解した方(Challenge CVE-2020-13777)　- ぼちぼち日記　　
  https://jovi0608.hatenablog.com/entry/2020/06/13/104905
- My answer for Challenge CVE-2020-13777(Japanese):
  - Challenge CVE-2020-13777の応募用紙  
  https://gist.github.com/prprhyt/548ba3148f3b1bbfa5c20edde60d6b75
- How to solved the problem of Challenge CVE-2020-13777(Japanese)
  - Challenge CVE-2020-13777に応募しました！  
https://atofaer.hatenablog.jp/entry/2020/07/03/132535
- An explanation of the reconnecting ability in TLS 1.3, which written by the organizer of Challenge CVE-2020-13777(Japanese)
  - GnuTLSの脆弱性でTLS1.3の再接続を理解する(Challenge CVE-2020-13777)  
  https://jovi0608.hatenablog.com/entry/2020/07/03/131719