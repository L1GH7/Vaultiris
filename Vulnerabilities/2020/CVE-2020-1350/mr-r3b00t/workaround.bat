reg add HKLM\SYSTEM\CurrentControlSet\Services\DNS\Parameters /v TcpReceivePacketSize /t REG_DWORD /d 0xFF00
net stop "DNS Server"
net start "DNS Server"

Get-ItemProperty -Path HKLM:\SYSTEM\CurrentControlSet\Services\DNS\Parameters\TcpReceivePacketSize
