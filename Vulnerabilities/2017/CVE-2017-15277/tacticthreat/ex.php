<?php
$rand=rand(1,555555);

echo '
<html>
    <head>
        <title>webpage.</title>
        <meta property="og:image" content="http://<AttackerServerDNSName>/red.php?'.$rand.'" />
    </head>
    <body>
        Hi. Welcome to my page.
    </body>
</html>
';
