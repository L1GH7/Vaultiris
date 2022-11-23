#CVE-2020-1350 Workaround (Local machine) - mRr3b00t
#Use at your own risk :P

$IsDNS = Get-Service -Name DNS
if($IsDNS.Status -eq "Running")
{
write-host "This is a DNS Server" -ForegroundColor Red

$CVE20201350_WorkArround =  Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\DNS\Parameters" -name TcpReceivePacketSize -ErrorAction SilentlyContinue

if($CVE20201350_WorkArround.TcpReceivePacketSize -ne 65280){

write-host "Not Applied" -ForegroundColor Red

    try{
    Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\DNS\Parameters" -Name "TcpReceivePacketSize" -Value 65280
    write-host "Workaround Applied" -ForegroundColor Green
    try
    {Restart-Service -Name DNS
    write-host "DNS RESTARTED" -ForegroundColor Green
    }catch{
    write-host "DNS RESTART FAILED" -ForegroundColor Red
    }

    }
    catch
    {
    write-host "Error Writing Key"
    }
    Finally
    {}
}
else
{
write-host "Workaround Already Applied" -ForegroundColor Cyan
}
}
else
{
write-host "THis is not a DNS Server." -ForegroundColor Cyan
}
