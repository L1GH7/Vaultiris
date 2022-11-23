# docker run -it --network ethermint-CVE-2021-25837_default ethermint /bin/sh
rm -rf ~/.ethermintcli
ethermintcli config keyring-backend test
echo "gossip ship high okay scorpion profit bridge cigar bargain clock burden jelly average float cruel insane decrease april basket flee usage text elephant safe" | ethermintcli keys add mykey --recover

ethermintcli config chain-id ethermint-1337
ethermintcli config output json
ethermintcli config indent true
ethermintcli config trust-node true
ethermintcli config node http://node:26657

# ethermintcli tx evm call 0x3f960fF4dC0bE6644eE933F2F418625beE48dfE2 0xa9059cbb000000000000000000000000766a0c4c52d64a1abc04783005b80114e3f2368d0000000000000000000000000000000000000000000000000de0b6b3a7640000 --from mykey
