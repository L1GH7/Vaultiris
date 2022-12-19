ip link set wlan0 down
iw wlan0 set monitor control
iw wlan0 set txpower fixed 3000
rfkill unblock all
ip link set  wlan0 up
bettercap -iface wlan0 -caplet pwnagotchi.cap
