echo "Checking if your system is vulnerable to CVE 2022-2639"

a=$(grep -H -e "^[[:blank:]]*blacklist [[:blank:]]*openvswitch$" /etc/modprobe.d/*)

if [[ -z $a ]]
then
        echo "you are vulnerable to CV2 2022-2639"
        read -p "DO YOU WANT TO REMEDIATE? (yes/no)" uservar
        if [ $uservar == yes ]
        then
               echo blacklist openvswitch >> /etc/modprobe.d/blacklist.conf
               echo "mitigation completed you are now save from CVE 2022-2639"
        elif [ $uservar == no ]
        then
                echo "exiting"
                exit
        else
                echo "cannot understand the command you have entered please try again"
                exit
        fi
else
        echo "you are already mitigated"
        echo "exiting"
        exit
fi
