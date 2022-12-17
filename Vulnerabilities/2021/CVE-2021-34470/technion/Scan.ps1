Set-StrictMode -Version 2

$SNC = (Get-ADRootDSE).SchemaNamingContext
$ob = "CN=ms-Exch-Schema-Version-Pt," + $SNC

try {
    $rangeupper = $(Get-ADObject $ob -Properties rangeUpper).rangeUpper
} catch {
    Write-Host "Exchange Schema not found. Not vulnerable"
    return
}
# Version list
# https://docs.microsoft.com/en-us/exchange/plan-and-deploy/prepare-ad-and-domains?view=exchserver-2019#exchange-active-directory-versions

# Exchange 2016 CU21	15334
# Exchange 2019 CU10	17003

if ($rangeupper -ge 17003) {
    write-host "SECURE: Detected Exchange 2019 CU10 Schema"
} elseif ($rangeupper -lt 17003 -and $rangeupper -ge 17000) {
    Write-Host "NOT SECURE: Detected Exchange 2019 schema of vulnerable version"
} elseif ($rangeupper -lt 17000 -and $rangeupper -ge 15334) {
    Write-Host "SECURE: Detected Exchange 2016 CU21 Schema"
} elseif ($rangeupper -lt 15334 -and $rangeupper -ge 15317) {
    Write-Host "NOT SECURE: Detected Exchange 2016 schema of vulnerable version"
} else {
    Write-Host "Unknown: Schema information not documented for earlier versions of Exchange"
}
