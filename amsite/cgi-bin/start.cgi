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
our $lang;

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

my %langhash;
my $content=header(-charset=>'UTF-8');

$dbh = tools::db_connect();
my ($userid, $usr)=tools::cookie_check($dbh);
$usr=param('user') unless $usr;

$lang=url_param('lang');
$lang='en' unless defined($lang);
open(TEM, "<$tools::dir_source/$lang/lang.txt");
while(my $line=<TEM>){
	chomp($line);
	next if $line!~/=/is;
	my($key, $value)=split(/=/, $line);
	$langhash{$key}=$value;
}
close(TEM);

my $temm=open_tem("index.tem");
my $login_user=<<FORM;
				<font size=-1>
				<h4><??MEM_LOGIN></h4>
				<span class=login>
				<form method='post' action='../cgi-bin/start.cgi?lang=<?LNG>'>
					<??USERNAME> <input type="text" name="user"></input>
					<br><??PWD> <input type="password" name="passwd"></input>
					<br><input type=submit value='<??LOG_IN>'></input>
				</form></span>
				<center><?MSG>&nbsp;</center></font>
FORM

#$content.="<p>Userid=$userid<p>";
my $logtext;
if(!$userid){
	$logtext=$login_user;
	if(defined($usr)){
		$usr=login($usr,param('passwd'));
		if($usr){
			my $session=set_cookie($usr);
			print redirect(-uri=>'start.cgi?lang='.$lang, -cookie=>$session);
			exit(0);
		}
		else{
			$logtext=~s/<\?MSG>/<font color=red><??INVALID_LOGIN><\/font>/s;
		}
	}
}
else{
	$logtext="<p align=center><??WELCOME>, $usr!</p><p align=right><a href='start.cgi?p=logout&lang=<?LNG>'><??LOGOUT></a></p>";
}
$temm=~s/<\?LOGIN_USER>/$logtext/is;
$content.=$temm;
my $page=url_param('p');
$page='' unless defined($page);

my $dync;
if($page eq 'geo'){
	require geo;
	$dync=geo::get_content($dbh, $userid,\%langhash);
}
if($page eq 'logout'){
	print redirect(-uri=>'start.cgi?lang='.$lang, -cookie=>cookie(-name=>'session',-value=>''));
	exit(0);
}

my $dyn='';
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
	$dync=open_tem($dyn);
	if($dync=~/<body>(.+?)<\/body>/is){
		$dync=$1;
	}
	else{
		$dync="<p align=center>$lang/$dyn missing</p>";
	}
}
$content=~s/\Q<?DYNAMIC_CONTENT>\E/$dync/is;
$content=~s/\Q<?LNG>\E/$lang/isg;
$content=~s/<\?\?([^>]+)>/$langhash{$1}/isge;
$content=~s/\Q<?DUMP>/&{\&Dump}/ise;
print $content;

sub open_tem{ # *.tem
	my $dyn=shift();
	my $fname="$tools::dir_source/$dyn";
	$fname="$tools::dir_source/$lang/$dyn" unless -f $fname;
	if(open(TEM, "<$fname")){
		undef $/;
		$dyn=<TEM>;
		$/="\n";
		close(TEM);
	}
	else{
		$dyn='';
	}
	return $dyn;
}

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
