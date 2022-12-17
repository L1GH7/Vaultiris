#!/usr/bin/perl
use strict;
use utf8;
use LWP::UserAgent;
use Data::Dumper;
use HTTP::CookieJar::LWP ();
use URI::Escape;

while(1){
    my $jar = HTTP::CookieJar::LWP->new;
    my $ua = new LWP::UserAgent (agent => 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:88.0) Gecko/20100101 Firefox/88.0', cookie_jar => $jar);#simulate a current windows firefox

    my $get_1 = $ua->get('https://icampus.th-wildau.de/corona-app-backend/sanctum/csrf-cookie');#get cookies

    my $post_1 = $ua->post('https://icampus.th-wildau.de/corona-app-backend/account',#get token
	"X-XSRF-TOKEN"=>getXSRF($jar),
	"Content-Type"=>"application/json;charset=utf-8",
	"X-Requested-With"=>"XMLHttpRequest",
	Content=>"{\"token\":null}");

    my $phone=getPhone();
    my $firstName=getFirstName();
    my $lastName=getLastName();
    my $post_2 = $ua->post('https://icampus.th-wildau.de/corona-app-backend/api/account/register',#register a new user
	"X-XSRF-TOKEN"=>getXSRF($jar),
	"Referer"=>'https://icampus.th-wildau.de/kontaktnachverfolgung/account',
	"Content-Type"=>"application/json;charset=utf-8",
	"X-Requested-With"=>"XMLHttpRequest",
	Content=>'{"first_name":"'.$firstName.'","last_name":"'.$lastName.'","telephone":"'.$phone.'","email":null,"accept":true}');

    if($post_2->code==200){#registered?
	print $post_2->header('x-ratelimit-remaining')."\n";
	saveUser($firstName,$lastName,$phone);
	my $post_3 = $ua->put('https://icampus.th-wildau.de/corona-app-backend/api/visit',
	    "X-XSRF-TOKEN"=>getXSRF($jar),
	    "Referer"=>'https://icampus.th-wildau.de/kontaktnachverfolgung/new',
	    "Content-Type"=>"application/json;charset=utf-8",
	    "X-Requested-With"=>"XMLHttpRequest",
	    Content=>'{"room":"15-K01","visited_on":"2021-05-09T22:01:00.000Z","exited_on":"2021-05-10T21:59:00.000Z"}');
	print $post_3->content."\n";
	print $post_3->code."\n";

    }
    sleep 5;#wait to not trigger any throttle protection
}


exit;

sub saveUser{
    if(open(F,">>users.txt")){
	print F $_[0].";".$_[1].";".$_[2]."\n";
	close(F);
    }
}

sub getXSRF{
    my $l_jar=$_[0];
    my $l_xsrf="";
    foreach($l_jar->cookies_for("https://icampus.th-wildau.de")){
	my %c=%{$_};
	#print $c{'name'}."\n";
	if($c{'name'} eq "XSRF-TOKEN"){
	    $l_xsrf=uri_unescape($c{'value'});
	}
    }
    if($l_xsrf eq ""){
	die("XSRF Token error!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
    return $l_xsrf;
}

sub getFirstName{
    my @data;
    if(open(F,"vornamen.dat")){
	@data=<F>;
	close(F);
    }
    my $r=@data[int(rand(@data))];chomp($r);
    return (split(/ /,$r))[0];
}

sub getLastName{
    my @data;
    if(open(F,"nachnamen.txt")){
	@data=<F>;
	close(F);
    }
    my $r=@data[int(rand(@data))];chomp($r);utf8::encode($r);
    return $r;
}


sub getPhone{
    my @pre=("0160","0170","0171","0175","0162","0172","0173","0174","0163","0177","0178","0176","0179");
    my $preN=@pre[int(rand(@pre))];
    my $amount=int(rand(2))+7;
    my $number="";
    while($amount!=0){
	my $digit=int(rand(10));
	$number.=$digit;
	$amount--;
    }
    #print $preN.$number."\n";
    return $preN.$number;
}
