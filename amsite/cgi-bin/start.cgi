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
our $dsn = "DBI:mysql:database=amax;host=localhost";
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

my $content=header().start_html(-title=>'Astromaximum location archives', -script=>$ADDCITY).<<BODY;
<p align="center"><b>Welcome to Astromaximum site!</b></p>

<table align=center valign=vcenter border=1>
	<form method='post' action='start.cgi'>
		<tr>
			<td>
				Username: <input type=text name=user></input>
			</td>
		</tr>
		<tr>
			<td>
				Password: <input type=password name=passwd></input>
			</td>
		</tr>
		<tr>
			<td align=center>
				<input type=submit value='Log in'></input>
			</td>
		</tr>
	</form>
</table>
BODY

my $usr=param('user');
if(defined($usr)){
	$usr=login($usr,param('passwd'));
	if($usr){
		my $session=set_cookie($usr);
		print redirect(-uri=>'http://astromaximum/cgi-bin/geo.cgi', -cookie=>$session);
	}
	else{
		print "$content<br><center><font color=red>Invalid username or password!</font></center>"
	}
}
else{
	print $content;
	print Dump();
}

print end_html;

sub login{ # user, passwd
	my($usr, $passwd)=@_;
	$dbh = DBI->connect($dsn, 'root', '');
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
	$md5->add(shift.time);
	my $dig=$md5->hexdigest;
	my $sth = $dbh->prepare("INSERT INTO sessions (name,user_id) VALUES (\"$dig\", $usr)");
	$sth->execute;
	$sth->finish;
	$dbh->disconnect;
	return cookie(-name=>'session',-value=>$dig,-expires=>'+1h');
}