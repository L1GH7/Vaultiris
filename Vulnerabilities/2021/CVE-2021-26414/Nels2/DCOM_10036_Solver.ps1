# DCOM_10036_Solver.ps1

# < SYPNOSIS > 
# This script was created to address the vulnerability described in CVE-2021-26414.
# Created by: Nelson Orellana
# Created on: Oct 24th, 2022

# Note:
# Important You must restart your device after setting this registry key for it to take effect. -- The script will auto restart after it is finished. 

# Additional Note:
# The registry values do not exist by default; they must be created. Windows will read it if it exists and will not overwrite it.

If (-NOT ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator))
{
  # Relaunch as an elevated process:
  Start-Process powershell.exe "-File",('"{0}"' -f $MyInvocation.MyCommand.Path) -Verb RunAs
  exit
}

# Set variables to indicate value and key to set for enabling/disabling hardening changes for CVE-26414
$RegistryPath = "HKLM:\SOFTWARE\Microsoft\Ole\AppCompat"
$Name = "RequireIntegrityActivationAuthenticationLevel"
$Value = '0x00000001'
# Test if key exists..
If (-NOT (Test-Path $RegistryPath)) {
    Write-Host "$RegistryPath does not exist. Script will now create it."
    New-Item -Path $RegistryPath -Force | Out-Null
}  
# Now to set the value
New-ItemProperty -Path $RegistryPath -Name $Name -Value $Value -PropertyType DWORD -Force

# Set variables to indicate value and key to to raise the activation authentication level
$RegistryPath1 = "HKLM:\SOFTWARE\Microsoft\Ole\AppCompat"
$Name1 = "RaiseActivationAuthenticationLevel"
$Value1 = '2'
# Test if key exists..
If (-NOT (Test-Path $RegistryPath1)) {
    Write-Host "$RegistryPath1 does not exist. Script will now create it."
    New-Item -Path $RegistryPath1 -Force | Out-Null
}  
# Now to set the value
New-ItemProperty -Path $RegistryPath1 -Name $Name1 -Value $Value1 -PropertyType DWORD -Force

