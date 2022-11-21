# CVE-2022-35841

This is only a small writeup with the theory, as this bug was collided on and I never did get around to properly testing this.

`EnterpriseAppMgmtSvc` is an interesting service implementing COM objects. Seems to date from Windows Phone and indeed, most of the COM interfaces implemented by this function are dead code which only works on Windows Phone - thanks OneCore!

The only coclass which seems to work by default on non-WCOS systems is `EnterpriseModernAppManager`. MS originally had a permission check (must be admin) on `EnterpriseModernAppManager::InstallApplication()`, but forgot to add any permission checks on the other methods in this coclass.

(Naturally, the patch for CVE-2022-35841 adds the permission checks for the other methods in `EnterpriseModernAppManager`.)

The interesting method here is `EnterpriseAppMgmtSvc::ProvisionApplication`, which stages (partially installs?) APPX packages from a passed XML-string configuration.

An arbitrary APPX package, via a couple of restricted capabilities, can be configured to install an NT service running as SYSTEM, via the `desktop6:Service` extension, and the `packagedServices` and `localSystemServices` restricted capability.

This is as far as I got. I naturally did not expect a collision and a patch today, so I never tested this in practise and therefore do not know what kind of signature the APPX would require.
