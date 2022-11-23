# Exploit Demo 

This guide will help you to install vulnerable component and perform the attack related to phpMyAdmin bug mentioned in [CVE-2018-1263](https://www.exploit-db.com/exploits/44928).

- [Description of CVE](#description-of-cve)
- [Why does the vulnerability exist](#why-does-the-vulnerability-exist)
- [How does the attack work](#how-does-the-attack-work)
- [How to install vulnerable component](#how-to-install-vulnerable-component)
- [How to perform the attack](#how-to-perform-the-attack)

## Description of CVE

This exploit is related to an issue which was discovered in phpMyAdmin version 4.8.x before 4.8.2. By exploiting the issue an attacker can perform remote code execution and local file inclusion on the server. The vulnerability is due to the portion of the code which is responsible for redirecting and loading pages within phpMyAdmin. The code has a faulty test for whitelisted pages which makes the attack possible.  An attacker must be authenticated, except in the "$cfg['AllowArbitraryServer'] = true" case (where an attacker can specify any host he/she is already in control of, and execute arbitrary code on phpMyAdmin) and the "$cfg['ServerDefault'] = 0" case (which bypasses the login requirement and runs the vulnerable code without any authentication).

## Why does the vulnerability exist

The vulnerability is caused by a validation bypass in the vulnerable path checking function. This vulnerability enables an authenticated remote attacker to execute arbitrary PHP code on the server.

There is a file inclusion in **index.php** of **phpMyAdmin** which can be triggered by providing a parameter named `target` in the URL and the portion of the code that validates the `target` parameter looks like the following
```php
$target_blacklist = array (
    'import.php', 'export.php'
);

// If we have a valid target, let's load that script instead
if (! empty($_REQUEST['target'])
    && is_string($_REQUEST['target'])
    && ! preg_match('/^index/', $_REQUEST['target'])
    && ! in_array($_REQUEST['target'], $target_blacklist)
    && Core::checkPageValidity($_REQUEST['target'])
) {
    include $_REQUEST['target'];
    exit;
}
// ...
```
In this code once the if condition is satisfied it executes `include $_REQUEST['target'];`. So, we just need to bypass the if condition to execute what we want.

Let us look at the if condition

1. The first two conditions says target parameter cannot be empty and it needs to be string.
2. The third condition restricts the target parameter from starting with index.
3. The fourth condition restricts that the target parameter should not be in `$target_blacklist`
    * The `$target_blacklist` is defined just before the if condition and it includes **import.php** and **export.php** meaning anything except these two pages are allowed.
4. The fifth and final condition is a whitelist check for the page validity using an existing function in Core class `Core::checkPageValidity($_REQUEST['target'])`.
    * As shown in the code snippet below the function `checkPageValidity` strips everything behind `?` from `$page` and checks if it is in the whitelist. The string after `?` is not part of the URL path. A sample whitelist is also shown in the code snippet.
    ```php
    public static function checkPageValidity(&$page, array $whitelist = [])
    {
        // ...
        $_page = mb_substr($page, 0, mb_strpos($page . '?', '?'));
        // example $whitelist == array('db_sql.php', 'sql.php', ...)
        if (in_array($_page, $whitelist)) {
            return true;
        }
        // ...
        return false;
    } 
    ```
    * The important point to note here is that the attacker has the complete control of `$page`, since it comes directly from `$_REQUEST['target']`. 

## How does the attack work
As mentioned previously the attacker has full control on `$page` in the `checkPageValidity` function via the `$_REQUEST['target']` parameter in the URL. Lets imagine that the attacker sends something like the following using `$_REQUEST['target']` parameter to `$page`
```php
$page = 'db_sql.php?/../../../../../../../../etc/passwd'
```
The `checkPageValidity` function then performs the following

1. First it splits the string at `?` and assigns the first part to `$page`. So in this example case the value of `$page = db_sql.php`.
2. Next it checks if `$_page`, i.e. **db_sql.php**, is in whitelist or not? Since it is in the whitelist the function returns True and goes back to `index.php`

Since the if condition in `index.php` is now True it executes the following line as shown in the index.php code snippet above
```php
include $_REQUEST['target'];
```
What happens next is

*  The statement above includes the non-splitted value of `$_REQUEST['target']` which means the following gets executed 
    ```
    GET /index.php?target=db_sql.php?/../../../../../../../../etc/passwd
    ```
* Since PHP magically converts path to **/../../../../../../../../etc/passwd** without checking whether `db_sql.php` exists or not, the **/../../../../../../../../etc/passwd** gets executed and the content of **/etc/passwd** file is sent in the response back to the attacker.

We can now use this to perform local file inclusion or even remote code execution to get a reverse shell back. Whenever we execute a query in phpMyAdmin, it creates a session file and stores in the /tmp directory with the query content. The session file named as sess_< SESSION ID >. The session ID can be easily found in the cookie using the inspect option of the browser.

So if we execute the following query in phpMyAdmin
```
SELECT '<?php phpinfo();exit;?>'
```

It will be stored in the session file. Lets imagine our session ID for the phpMyAdmin is `e15cffd3ab25a631136611fba9ca2042`

Then if we trigger the following address in the browser
```
http://your-ip:8080/index.php?target=db_sql.php?/../../../../../../../../tmp/sess_e15cffd3ab25a631136611fba9ca2042
```

Then phpMyAdmin will try to load the session page and since the page contains the php code provided through the query, the php code will be executed and in this particular example we will see phpinfo in the page loaded by the browser. Using this technique we can execute any arbitrary code on the remote server.

## How to install vulnerable component
The exploit infrastructure requires vulnerable version of phpMyAdmin and mysql. We will use docker containers to install the required components. For vulnerable version of phpMyAdmin we will use prebuilt docker environnement from [Vulhub](https://github.com/vulhub/vulhub) and for mysql we will use latest official version from [dockerhub](https://hub.docker.com/). The installation script is provided as a docker compose yml script which can be found in the repository. 

We assume that the intended machine has **docker** and **docker-compose** installed. If not then please refer to the docker [documentation](https://docs.docker.com/get-docker/) to install them. Once docker is installed, perform the following steps to setup the vulnerable component:

First clone the repository in your intended machine and navigate to the cloned directory
```bash
git clone git@gits-15.sys.kth.se:msnkhan/exploit-demo.git
cd exploit-demo
```
Then run the docker compose script using the following command
```bash
sudo docker-compose up -d
```
Once installation process is finished docker will expose phpMyAdmin page on port **8080** of your machine. You can check by opening the page in your browser using the following address format
```
http://your-machine-ip:8080
```
If the installation process is successful you should see a page like the following

![phpMyAdmin_homepage](https://drive.google.com/uc?id=1520feaCSqgFcZipBiBJ9xTvb850_omjO)


## How to perform the attack
The following video is a tutorial on how to perform the attack

**Video Link:** https://drive.google.com/file/d/1UbLGEwMYswdrRAOMJLd0mFHbsNl1aPvt/preview

## References
[1] https://www.exploit-db.com/exploits/44928

[2] https://github.com/vulhub/vulhub

[3] https://docs.docker.com/get-docker/

