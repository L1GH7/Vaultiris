#!/bin/sh

if [ -z ${1+x} ]; then
  echo "Usage: start.sh <wifi interface to setup AP on>"
  exit 1
fi

./hostapd /dev/stdin -d <<- EOF
ssid=TESTSSID
channel=1
logger_stdout=-1
logger_stdout_level=4
hw_mode=g
ieee80211n=1
wmm_enabled=1
interface=$1
auth_algs=1
wpa=2
wpa_key_mgmt=WPA-PSK
wpa_passphrase=abcdabcd
rsn_pairwise=CCMP
EOF
