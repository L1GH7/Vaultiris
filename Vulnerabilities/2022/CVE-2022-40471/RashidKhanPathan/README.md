# CVE-2022-40471

Remote code execution via unrestricted file upload vulnerability in the Clinic's Patient Management System v 1.0

![image](https://static.wixstatic.com/media/cf57b8_8928645050c94227991adee114f384e0~mv2.png/v1/fill/w_740,h_383,al_c,q_85,usm_0.66_1.00_0.01,enc_auto/cf57b8_8928645050c94227991adee114f384e0~mv2.png)

# Technical description:

Remote Code Execution in Clinic's Patient Management System v 1.0 allows Attacker to Upload arbitrary php webshell via profile picture upload functionality in users.php

## Affected components - Source Code

in following Source Code we can see that developer directly allows to upload any media files without restricting an a specific extenstion which means we can upload any extensions file there which is not okay for security reasons and using this functionality flaw, attacker could upload malicious webshell to gain access complete server with root privileges

###### Mitigations: Please Restrict other extenstions while uploading an image file for profile picture upload functionality and allow only .jpg .png extensions which are only image related

Vulnerable Page - users.php

```php
  // users.php
  $status = move_uploaded_file(
    $_FILES["profile_picture"]["tmp_name"],
    'user_images/' . $targetFile
  );

  if ($status) {
    try {
      $con->beginTransaction();

      $query = "INSERT INTO `users`(`display_name`,
`user_name`, `password`, `profile_picture`)
VALUES('$displayName', '$userName', '$encryptedPassword', '$targetFile');";

      $stmtUser = $con->prepare($query);
      $stmtUser->execute();

      $con->commit();

      $message = 'user registered successfully';
    } catch (PDOException $ex) {
      $con->rollback();
      echo $ex->getTraceAsString();
      echo $ex->getMessage();
      exit;
    }
  } else {
    $message = 'a problem occured in image uploading.';
  }

  header("location:congratulation.php?goto_page=users.php&message=$message");
  exit;
}
```

<!-- ![image](../CVE-2022-40471/Screenshot%202022-10-15%20103606.png) -->

# CVE-2022-40471.py usage -

```sh
# Upload a simple webshell to the target machine -
python3 CVE-2022-40471.py <target_ip> <target_port> <target_uri> <username> <password>
```

## Example -

```sh
python CVE-2022-40471.py 127.0.0.1 80 /pms/ UserName Password
```

# Proof of concept (Poc) -

![Screenshot 2022-10-13 053055](https://user-images.githubusercontent.com/65374016/195474325-7c20861d-c64d-470c-8d03-3fb1078ee3da.png)

# References -

https://drive.google.com/file/d/1m-wTfOL5gY3huaSEM3YPSf98qIrkl-TW/view?usp=sharing

https://www.sourcecodester.com/php-clinics-patient-management-system-source-code

https://www.sourcecodester.com/sites/default/files/download/oretnom23/php-cpms.zip

# Discovered & Developed by -

RashidKhan Pathan (iHexCoder), 9 September 2022.
Twitter: @itRashid
