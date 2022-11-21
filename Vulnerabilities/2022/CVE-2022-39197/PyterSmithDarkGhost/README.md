# CVE-2022-39197-POC  
  
---

## Vulnerabilidade Intro
De acordo com [Update Log](https://www.cobaltstrike.com/blog/out-of-band-update-cobalt-strike-4-7-1/) da última versão 4.7.1 lançada oficialmente pela CobaltStrike em 20 de setembro, a versão teamserver(<=4.7) possui vulnerabilidade XSS, o que pode causar RCE.
> Fomos contatados por um pesquisador independente chamado "Beichendream" para nos informar sobre uma vulnerabilidade XSS encontrada nos servidores da equipe. Isso permitiria que um invasor defina um nome de usuário malformado na configuração do Beacon, permitindo que eles executassem o código RCE remotamente.

---

## POC Intro

Atualmente, este POC só pode implementar imagens de bomba do servidor de equipe.
Ferramenta de análise integrada `CobaltStrikeParser` para obter uma paragem online.

---

## Usage

```
pip3 install -r requirements.txt
python3 cve-2022-39197-poc.py -i http://172.16.12.2:3000/logo.png -b beacon.exe

```

---
