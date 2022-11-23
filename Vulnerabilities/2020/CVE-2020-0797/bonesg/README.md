# CVE-2020-0796

Détecter les ordinateurs vulnérables à la faille CVE-2020-0796, Remote Code Execution du protocole SMB v3.1.1 (SMBv3).

**Comment utiliser :**
- Script Python: *python3 check-smb-v3.11.py [insérer IP]*
- Script Shell/Bash: *./check-smb-v3.11 [insérer IP]*

**Systèmes infectables :**
- Windows 10 [Version 1903] (x86)
- Windows 10 [Version 1903] (ARM64)
- Windows 10 [Version 1903] (x64)
- Windows 10 [Version 1909] (x86)
- Windows 10 [Version 1909] (ARM64)
- Windows 10 [Version 1909] (x64)
- Windows Server [Version 1903] (Server Core installation)
- Windows Server [Version 1909] (Server Core installation)

**Comment marche la vulnérabilité ?**
> Microsoft est au courant de l'existence d'une vulnérabilité de type RCE (pour Remote Code Execution) du protocole Microsoft Server Message Block 3.1.1 (ou SMBv3). Un attaquant ayant exploité la vulnérabilité peut exécuter du code sur le serveur et/ou client SMB de la cible.

> - Pour exploiter la vulnérabilité sur un serveur SMB, un attaquant non authentifié peut envoyer un certain packet sur le serveur SMBv3 ciblé.

> - Pour exploiter la vulnérabilité sur un client SMB, un attaquant non authentifié doit configurer un serveur SMBv3 malicieux et convaincre un utilisateur de s'y connecter.

**Comment se défendre de la vulnérabilité ?**
> - Télécharger une mise à jour de Microsoft patchant la vulnérabilité : http://www.catalog.update.microsoft.com/Search.aspx?q=KB4551762
- Ou désactiver la compression SMBv3 afin d'éviter des attaquants non authentifiés d'exploiter la vulnérabilité (à faire dans un terminal PowerShell) :
> Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\LanmanServer\Parameters" DisableCompression -Type DWORD -Value 1 -Force
