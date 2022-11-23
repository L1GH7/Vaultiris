var=$(openssl version | grep -E [3]\.[0\.*[0-6]*])

if [[ -n $var ]]
then
        echo "you are vulnerable to the openSSL vulnerability cve-2022-2274 "
else
        echo "you are safe from open SSL vulnerability cve-2022-2274"
fi
