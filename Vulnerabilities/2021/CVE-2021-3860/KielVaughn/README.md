# CVE-2021-38603

A stored cross site scripting vulnerability is present on the Profile edit page in the **Information:** field for each user.

## http://\<hostname/server ip\>/core/admin/profil.php

### Vulnerable Fields:

- Information:

![User Profile Page](PluXML_Profile.png)

Once inserted, XSS can be triggered by visiting any page/article created by that particular user.

![Profile XSS](PluXML_Profile_Stored_XSS.png)
