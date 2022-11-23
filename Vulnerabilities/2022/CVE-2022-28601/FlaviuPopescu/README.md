# CVE-2022-28601

A Two-Factor Authentication (2FA) bypass vulnerability in "Simple 2FA  Plugin for Moodle" by LMS Doctor

Vulnerability Details

Risk : Medium

Vendor: [LMS Doctor - Simple 2 Factor Authentication Plugin For Moodle](https://www.lmsdoctor.com/simple-2-factor-authentication-plugin-for-moodle)

Disclosed by: [Flaviu Popescu](https://flaviu.io)

Description:
Two-Factor Authentication Bypass vulnerability in The Simple 2FA Plugin for Moodle, by "LMS Doctor" allows attackers to overwrite the phone number attached to an account.
Thus allowing them to bypass the second stage of the verification.

Proof of concept:
The example below shows the initial login process using a self-registered account.

POST /login/index.php

![image](https://user-images.githubusercontent.com/62330554/167459894-74467c2c-90ed-4f82-86f5-f90a72b44125.png)

After entering their username and password, the website sends the account owner a six-digit code to their mobile device, as shown below:

POST /auth/simple2fa/confirm.php

![image](https://user-images.githubusercontent.com/62330554/167459999-1ac7edda-d766-41f7-a92b-4bd1e595cb9d.png)

If an attacker then force browses to the following URL instead of providing the 2FA code, they are able to update the phone number registered to the account.

POST /auth/simple2fa/profile.php

![image](https://user-images.githubusercontent.com/62330554/167460043-69fd154b-b7d4-4b49-9c0b-ae37be249dca.png)

A new phone number belonging to the attacker is added to the account. The login process is then repeated, but this time the six-digit pin code will be received on the attacker's device.
The newly generated six-digit pin code is then passed into the 2FA authentication portal which now shows the attacker's phone number.

POST /auth/simple2fa/confirm.php

![image](https://user-images.githubusercontent.com/62330554/167460094-03585336-93e7-41a6-a6ca-7773dbe1dab8.png)

The attacker is then granted access to the website effectively bypassing the second stage of the authentication process.
