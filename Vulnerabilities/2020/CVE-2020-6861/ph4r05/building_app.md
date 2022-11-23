```bash
git clone --recursive https://github.com/ph4r05/blue-app-monero.git
cd blue-app-monero/
docker run -t -i -v `PWD`:/code lukechilds/ledger-sdk /bin/bash
```

In docker:
```bash
apt update
apt install python3-pip 
pip3 install image

# Fetch the correct SDK, firmware 1.6.0 for Nano S
cd /opt && \
    mv bolos-sdk bolos-sdk-old && \
    git clone --recursive https://github.com/LedgerHQ/nanos-secure-sdk.git bolos-sdk && \
    cd bolos-sdk && \
    git checkout tags/nanos-160 && \
    cd /code

export TARGET_NAME=TARGET_NANOS 
export TARGET_ID=0x31100004
export GCCPATH=/opt/bolos-env/gcc-arm-none-eabi-5_3-2016q1/bin/
export CLANGPATH=/opt/bolos-env/clang-arm-fropi/bin/

# By default the third-party apps do not have AES enabled.
TARGET_OPTS="IOCUSTOMCRYPT USE_DEMO_ADDR" make
```

Native:
```bash
CDIR=`pwd`
docker ps  # get Docker ID
docker cp 2984cdaf922d:/code/blue-app-monero/bin/app.bin .
docker cp 2984cdaf922d:/code/blue-app-monero/bin/app.elf .
pip3 install ledgerblue image

# Fetch SDK also in native
cd /tmp && \
    mv bolos-sdk bolos-sdk-old && \
    git clone --recursive https://github.com/LedgerHQ/nanos-secure-sdk.git bolos-sdk && \ 
    cd /tmp/bolos-sdk && \
    git checkout tags/nanos-160 && \
    export BOLOS_SDK=/tmp/bolos-sdk
cd $CDIR

# Install compiled application
python3 -m ledgerblue.loadApp --path "2147483692/2147483776" --curve secp256k1 --tlv --targetId 0x31100004 --targetVersion="1.6.0" \
    --delete --fileName bin/app.hex --appName "MoneroDemo" --appVersion 1.4.2 \
    --dataSize $((0x`cat debug/app.map |grep _envram_data | tr -s ' ' | cut -f2 -d' '|cut -f2 -d'x'` - 0x`cat debug/app.map |grep _nvram_data | tr -s ' ' | cut -f2 -d' '|cut -f2 -d'x'`)) `ICONHEX=\`python3 $BOLOS_SDK/icon3.py --hexbitmaponly images/icon_monero.gif  2>/dev/null\` ; [ ! -z "$ICONHEX" ] && echo "--icon $ICONHEX"`  --appFlags 0x240

# python3 -m ledgerblue.deleteApp --targetId 0x31100004 --appName MoneroDemo
# python3 -m ledgerblue.loadApp --targetId 0x31100004 --apdu --fileName app.hex --appName MoneroDemo --appVersion 1.4.2 --tlv --path "2147483692/2147483776" --curve secp256k1 --appFlags 0x240
# python3 -m ledgerblue.loadApp --targetId 0x31100004 --apdu --fileName app.hex --appName MoneroDemo --appFlags 0x00 --icon "" --appVersion 1.4.2 --tlv
```

















