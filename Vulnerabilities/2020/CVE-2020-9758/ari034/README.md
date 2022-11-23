CVE-2020-9758


> [Description]
> An issue was discovered in chat.php in LiveZilla Live Chat 8.0.1.3
> (Helpdesk). A blind JavaScript injection lies in the name parameter.
> Triggering this can fetch the username and passwords of the helpdesk
> employees in the URI. This leads to a privilege escalation, from
> unauthenticated to user-level access, leading to full account
> takeover. The attack fetches multiple credentials because they are
> stored in the database (stored XSS). This affects the mobile/chat URI
> via the lgn and psswrd parameters.
>
> ------------------------------------------
>
> [Additional Information]
> The leakage of credentials through the URI may be the result of the autologin feature.
> Also more parameters in the chat.php form may be vulnerable.
>
> ------------------------------------------
>
> [Vulnerability Type]
> Cross Site Scripting (XSS)
>
> ------------------------------------------
>
> [Vendor of Product]
> Livezilla
>
> ------------------------------------------
>
> [Affected Product Code Base]
> Livechat Helpdesk - 8.0.1.3
>
> ------------------------------------------
>
> [Affected Component]
> Input URL : https://livechat.example.com/chat.php
> Vulnerable Parameter : name
> Affected URL : https://livechat.example.com/mobile/chat?lgn=base64_encoded(username)&psswrd=base64_encoded(password)
>
> ------------------------------------------
>
> [Attack Type]
> Remote
>
> ------------------------------------------
>
> [Impact Escalation of Privileges]
> true
>
> ------------------------------------------
>
> [Impact Information Disclosure]
> true
>
> ------------------------------------------
>
> [Attack Vectors]
> Blind Unauthenticated Stored XSS
>
> ------------------------------------------
>
> [Reference]
> https://www.livezilla.net
>
> ------------------------------------------
>
> [Discoverer]
> Arihant Singh
