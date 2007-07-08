#!/usr/bin/perl

use strict;
use warnings;
use Digest;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
use tools;

$CGI::POST_MAX=1024 * 10;  # max 100K posts
$CGI::DISABLE_UPLOADS = 1;  # no uploads

our $cur_country='';
our $dbh;

my $ADDCITY=<<ADDCITY;
function city_add(cname){
	selc=document.getElementById("selcit");
	chkcol=document.getElementById("chkcit");
	sc=document.getElementById("sc");
	out="";
	for(i=0; i<chkcol.all.length; i++){
		opt=chkcol.all.item(i);
		if(opt.checked && sc.value.indexOf("."+opt.id+".")<0){
			out=out+"<input type=checkbox id="+opt.id+">"+opt.value+", "+cname+"</input><br>";
			sc.value=sc.value+opt.id+".";
		}
	}
	selc.innerHTML=selc.innerHTML+out;
};

function show_country(country){
	if(document.forms('main').item('cid').value!=country){
		document.forms('main').item('cid').value=country;
		document.forms('main').submit();
	}
};

function city_del(){
	chkcol=document.getElementById("selcit");
	sc=document.getElementById("sc");
	oldsc=sc.value;
	sc.value=".";
	for(i=0; i<chkcol.all.length; i++){
		opt=chkcol.all.item(i);
		if(!opt.checked && opt.id!=""){
			sc.value=sc.value+opt.id+".";
		}
	}
	if(oldsc!=sc.value){
		document.forms('main').submit();
	}
};
ADDCITY

open(TEM, "<$tools::dir_source/index.tem");
my @tem=<TEM>;
close(TEM);
my $temm="@tem";
my $login_user=<<FORM;
				<h5>Subscribers only:</h5>
				<form method='post' action='../cgi-bin/start.cgi'>
					<p align=center><font size=-1>Username:</font>					
					<input type=text name=user></input></p>
					<p align=center><font size=-1>Password:</font>					
					<input type=password name=passwd></input></p>
					<p align=center>
					<input type=submit value='Log in'></input></p>
				</form>
FORM

my $content=header();

$dbh = tools::db_connect();
my ($userid, $usr)=tools::cookie_check($dbh);
$usr=param('user') unless $usr;
#$content.="<p>Userid=$userid<p>";
my $logtext;
if(!$userid){
	$logtext=$login_user;
	if(defined($usr)){
		$usr=login($usr,param('passwd'));
		if($usr){
			my $session=set_cookie($usr);
			print redirect(-uri=>'start.cgi', -cookie=>$session);
			exit(0);
		}
		else{
			$logtext.="<center><font color=red>Invalid username or password!</font></center>";
		}
	}
}
else{
	$logtext="<table width=100%><tr><td>Welcome, $usr</td><td><p align=right><a href='start.cgi?p=logout'>Logout</a></p></td></tr></table>";
}
$temm=~s/<\?LOGIN_USER>/$logtext/is;
$content.=$temm;
my $page=param('p');
$page='' unless defined($page);
my $dyn='';
if($page eq 'geo'){
	require geo;
	$dyn=geo::get_content($dbh, $userid);
}
if($page eq 'logout'){
	print redirect(-uri=>'start.cgi', -cookie=>cookie(-name=>'session',-value=>''));
	exit(0);
}

$dyn="!!home.tem" if $page eq '';
$dyn="!!features.tem" if $page eq 'feat';
$dyn="!!links.tem" if $page eq 'links';
$dyn="!!screens.tem" if $page eq 'scr';
$dyn="!!contact.tem" if $page eq 'contact';
$dyn="!!requirements.tem" if $page eq 'req';
$dyn="!!demo.tem" if $page eq 'demo';
$dyn="!!test.tem" if $page eq 'test';
$dyn="!!order.tem" if $page eq 'order';

if($dyn=~s/!!(.+)/$1/is){
	if(open(TEM, "<$tools::dir_source/$dyn")){
		@tem=<TEM>;
		close(TEM);
		$dyn="@tem";
		if($dyn=~/<body>(.+?)<\/body>/is){
			$dyn=$1;
		}
	}
	else{
		$dyn="<p align=center>$dyn missing</p>";
	}
}
$content=~s/\Q<?DYNAMIC_CONTENT>\E/$dyn/is;

#$content=~s/\Q<?DUMP>/&{\&Dump}/ise;
print $content;

sub login{ # user, passwd
	my($usr, $passwd)=@_;
	$dbh = tools::db_connect();
	my $stat="SELECT id FROM customers WHERE name=\"$usr\" AND hash=\"$passwd\"";
	my $sth = $dbh->prepare($stat);
	$sth->execute;
	my $success=0;
	if($sth->rows()==1){
		$success=($sth->fetchrow_array)[0];
	}
	$sth->finish;
	return $success;
}

sub set_cookie{
	my $md5  = Digest->new("MD5");
	$md5->add(shift().time);
	my $dig=$md5->hexdigest;
	my $sth = $dbh->prepare("INSERT INTO sessions (name,user_id) VALUES (\"$dig\", $usr)");
	$sth->execute;
	$sth->finish;
	$dbh->disconnect;
	return cookie(-name=>'session',-value=>$dig,-expires=>'+1h');
}
