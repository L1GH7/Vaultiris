<html>
<body>
<?php
$f = fopen("result.html", "w") or die("Unable to open file!");
fwrite($f, file_get_contents("php://input"));
fclose($f);
?>
</body>
</html>
