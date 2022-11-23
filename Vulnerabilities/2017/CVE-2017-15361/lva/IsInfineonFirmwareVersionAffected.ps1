# This script has been copied from https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/ADV170012
# It has been made available on GitHub for easier reference and reuse.

# IMPORTANT: THIS MUST BE EXECUTED AS ADMINISTRATOR!!

# The manufacturer ID of Infineon.
$IfxManufacturerIdInt = 0x49465800 # 'IFX'

function IsInfineonFirmwareVersionAffected ($FirmwareVersion)
{
	$FirmwareMajor = $FirmwareVersion[0]
	$FirmwareMinor = $FirmwareVersion[1]
	switch ($FirmwareMajor)
	{
		4 { return $FirmwareMinor -le 33 -or ($FirmwareMinor -ge 40 -and $FirmwareMinor -le 42) }
		5 { return $FirmwareMinor -le 61 }
		6 { return $FirmwareMinor -le 42 }
		7 { return $FirmwareMinor -le 61 }
		133 { return $FirmwareMinor -le 32 }
		default { return $False }
	}
}

function IsInfineonFirmwareVersionSusceptible ($FirmwareMajor)
{
	switch ($FirmwareMajor)
	{
		4 { return $True }
		5 { return $True }
		6 { return $True }
		7 { return $True }
		133 { return $True }
		default { return $False }
	}
}

$Tpm = Get-Tpm
$ManufacturerIdInt = $Tpm.ManufacturerId
$FirmwareVersion = $Tpm.ManufacturerVersion -split "\."
$FirmwareVersionAtLastProvision = (Get-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\TPM\WMI" -Name "FirmwareVersionAtLastProvision" -ErrorAction SilentlyContinue).FirmwareVersionAtLastProvision

if (!$Tpm)
{
	Write-Host "No TPM found on this system, so the issue does not apply here."
}
else
{
	if ($ManufacturerIdInt -ne $IfxManufacturerIdInt)
	{
		Write-Host "This non-Infineon TPM is not affected by the issue."
	}
	else
	{
		if ($FirmwareVersion.Length -lt 2)
		{
			Write-Error "Could not get TPM firmware version from this TPM."
		}
		else
		{
			if (IsInfineonFirmwareVersionSusceptible($FirmwareVersion[0]))
			{
				if (IsInfineonFirmwareVersionAffected($FirmwareVersion))
				{
					Write-Host ("This Infineon firmware version {0}.{1} TPM is not safe. Please update your firmware." -f [int]$FirmwareVersion[0], [int]$FirmwareVersion[1])
				}
				else
				{
					Write-Host ("This Infineon firmware version {0}.{1} TPM is safe." -f [int]$FirmwareVersion[0], [int]$FirmwareVersion[1])
					if (!$FirmwareVersionAtLastProvision)
					{
						Write-Host ("We cannot determine what the firmware version was when the TPM was last cleared. Please clear your TPM now that the firmware is safe.")
					}
					elseif ($FirmwareVersion -ne $FirmwareVersionAtLastProvision)
					{
						Write-Host ("The firmware version when the TPM was last cleared was different from the current firmware version. Please clear your TPM now that the firmware is safe.")
					}
				}
			}
			else
			{
				Write-Host ("This Infineon firmware version {0}.{1} TPM is safe." -f [int]$FirmwareVersion[0], [int]$FirmwareVersion[1])
			}
		}
	}
}