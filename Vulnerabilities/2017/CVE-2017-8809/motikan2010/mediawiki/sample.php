<?php

if ( !is_null($_GET['download']) ) {
    if ( $_GET['download'] === '1' ) {
        header('Content-Disposition: inline; filename="original_name.txt"');
    }
    exit;
}

echo '<pre><code>' . htmlspecialchars(file_get_contents('./sample.php')) . '</code></pre>';

?>

<hr/>

<!DOCTYPE html>
<html>
<head>
    <title></title>
</head>
<body>
    <p>↑ code</p>
    <a href="./sample.php?download=1" download="malicious.cmd">Enable "Content-Disposition" Header Download</a><br/>
    <a href="./sample.php?download=0" download="malicious.cmd">  None "Content-Disposition" Header Download</a>
</body>
</html>

