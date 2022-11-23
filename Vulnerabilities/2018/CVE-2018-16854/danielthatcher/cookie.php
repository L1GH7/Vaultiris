<?php
    $req = curl_init("http://moodle.lab.local/login/index.php");
    curl_setopt($req, CURLOPT_ENCODING, "");
    curl_setopt($req, CURLOPT_HEADER, true);
    curl_setopt($req, CURLOPT_POST, true);
    curl_setopt($req, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($req, CURLOPT_POSTFIELDS, "anchor=&username=attacker&password=Password1!");
    $result = curl_exec($req);
    curl_close($req);

    // From https://stackoverflow.com/questions/895786/how-to-get-the-cookies-from-a-php-curl-into-a-variable
    preg_match_all('/^Set-Cookie:\s* MoodleSession=([^;]*)/mi', $result, $matches);
    echo $matches[1][0];
?>
