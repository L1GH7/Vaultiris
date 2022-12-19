This repository contains the description of the vulnerability found (CVE-2020-28874) in [ProjectSend](https://www.projectsend.org/yes) r1270 release. 

Thank you to my colleagues from  Entelgy Innotec Security for pushing me to improve!

# CVE-2020-28874: Privilege Escalation
## Description
The vulnerability is in the reset password component, an attacker can change the password of any known user by his username, such as admin, without a valid token.

This is possible because the backend does not clean the $user_data with the user data loaded from the username provided.
```php
 if (!empty($_GET['token']) && !empty($_GET['user'])) {
        $got_token	= $_GET['token'];
        $got_user	= $_GET['user'];

        /**
         * Get the user's id
         */
        $user_data = get_user_by_username($got_user);
        ....
        }
```
And use the state of that variable to continue the process to reset the password.

```php
    if ($_POST) {
        /**
         * Clean the posted form values.
         */
        $form_type = encode_html($_POST['form_type']);
        
        switch ($form_type) {
        ----
            case 'new_password':
                if (!empty($user_data['id'])) {
                    $reset_password_new = $_POST['password'];
                    ...
                }
        ...
        }
```
### Attack
To do that, the attacker needs to craft a POST request, with an invalid parameter "token" and the "user" parameter with the target username in the query string, also need to add the “form_type” as “new_password”, “password” with the desire new password, the csrf_token with a valid one (from the login form, the reset password form does not provide that) in the body part.

In this repository, you can find a python script to exploit the vulnerability and a docker container to test it. 
#### Usage
```sh
git clone https://github.com/varandinawer/CVE-2020-28874_CVE-2020-28875.git
cd ./CVE-2020-28874_CVE-2020-28875/docker-r1270
docker-compose up 
#To finish the process of ProjectSend installation, go to http://localhost in the browser and follow the indication to setup an admin account
cd .. 
python CVE-2020-28874.py --url http://localhost --user admin --pwd newAdminP4ssword*
```
## Fix
Upgrade to version [r1295](https://www.projectsend.org/change-log/) or more. 



