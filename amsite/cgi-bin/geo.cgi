#!/usr/bin/perl

use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 10;  # max 100K posts
$CGI::DISABLE_UPLOADS = 1;  # no uploads
use tools;
use amtools;

our $cur_country='';


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

my $dsn = "DBI:mysql:database=amax;host=localhost";
my $mode=defined(param('cid'));
my @sel_cities=param('Sel_cities');
my $dbh = DBI->connect($dsn, 'root', '');

my $userid=tools::cookie_check($dbh);

my $cnum=param('cid');
my $defyear=2007;

print header, start_html(-title=>'Astromaximum location archives', -script=>$ADDCITY);

print tools::adm_panel();

print start_form(-name=>'main', -method=>'post'), "<table width=100% border=1><tr valign=top><td><font color='red'><i>Step 1:</i></font><br><b>Year:</b> ",
popup_menu(-name=>'year', -values=>[qw(2005 2006 2007 2008)], -default=>$defyear, 
	#-onchange=>"javascript:window.refresh()"
	), "</td>";
print "<td rowspan=2>", selected_cities(), "</td></tr>";
print "<tr><td>", country_header(), city_selector(), "</td>";


print "</tr></table>";
print hidden(-name=>'sc', -default=>'.');
print hidden(-name=>'cid', -default=>'');
end_form();
#print Dump();
#print join('.',@sel_cities);

if(param('Action') eq 'Get data' && param('sc')=~/\d/){
	my $id=create_jar(param('year'), param('sc'));
	my $url='http://astromaximum/cgi-bin/data?r='.$id;
	print "<p><center><font color='red'><i>Step 4:</i></font>";
	print "<h4>Download to PC:</h4>";
	print "<b>JAR link: <a href=\'$url\'>$id</a><br><br>";
	$url=~s/\?r/\?d/is;
	print "JAD link: <a href=\'$url\'>$id</a><br><br></b>";
	$url=~s/\?d/\?t/is;
	print "<h4>Download to phone:</h4>";
	print "<b>Direct link: <a href=\'$url\'>$id</a><br>";
	print "<br><font color='red'>Attention: links are valid within next 2 hours!</font></b></center>";
}
print end_html;
$dbh->disconnect;

sub country_header{
	my $res="<font color='red'><i>Step 2:</i></font><br><b>Country: </b>";
	my $stcou = $dbh->prepare("SELECT countries.id, countries.name FROM countries ORDER BY countries.name");
	$stcou->execute;
	while(my @row = $stcou->fetchrow_array){
		if(!$cnum){
			$cnum=$row[0];
			param('cid',$cnum);
		}
		if($row[0]==$cnum){
			$cur_country=$row[1];
			$row[1]="<b>$row[1]</b>" ;
		}
		$res.="<a href='#' onclick=\"show_country($row[0])\">$row[1]</a>\n"; 
	}
	$stcou->finish;
	return $res;
}

sub city_selector{
	my $sth = $dbh->prepare(
		"SELECT cities.id, cities.name FROM cities,countries,locations WHERE country_id=".
		"$cnum AND countries.id=country_id AND city_id=cities.id AND year=".
		param('year')." ORDER BY cities.name");
	$sth->execute;
	my $res="<br><br><b>Cities:</b><br><div id=chkcit>";
	my $i=0;
	while(my @row = $sth->fetchrow_array){
		$res.="<input type=checkbox id=$row[0] value='$row[1]'>$row[1]</input><br>\n"; 
		$i++;
	}
	$res.="</div>";
	$sth->finish;
	$res.=button(-value=>'Add cities', -onClick=>"city_add(\"$cur_country\")");
	return $res;
}

sub selected_cities{
	my $rs=restored_selection(param('sc'));
	my $res="<h4 align=center>Selected cities:</h4>";
#	if($rs){
		$res.="<div align=right>".button(-value=>'Delete selected', -onClick=>"city_del()")."</div>".
			"<div id=selcit>".$rs."</div><p align=center><font color='red'><i>Step 3:</i></font> ";
		$res.=submit('Action','Get data')."</p>";
#	}
#	else{
#		$res.="<center><b><i>No cities selected.</i></b></center>";
#	}
	return $res;
}

sub restored_selection{ # ids
	my $ids=shift;
	$ids=~/^\.(.+?)\.?$/is;
	$ids=$1;
	$ids=~s/\./\,/isg;
	my $res='';
	my $stat="SELECT cities.id, cities.name, countries.name FROM cities,countries WHERE cities.id IN ($ids) and countries.id=country_id ORDER BY countries.name,cities.name";
#	print $stat;
	my $sth = $dbh->prepare($stat);
	$sth->execute;
	while(my @row = $sth->fetchrow_array){
		$res.="<input type=checkbox id=$row[0]></input>$row[1], $row[2]<br>\n";	
	}
	$sth->finish;
	return $res;
	
}

sub create_jar{ # $year, $city_ids
	my($year, $ids, $outfile)=@_;
	$ids=~/^\.(.+?)\.?$/is;
	$ids=$1;
	$ids=~s/\./\,/isg;
	my ($dir,$fn)=amtools::random('../files','.r');
	my $srcdir="../source/$fn";
	mkdir $srcdir;
	open(INF, "<../source/template.jad") or die "No file";
	my @data=<INF>;
	close(INF);
	my $template=join("",@data);
	@data=();
	$fn=~/(\d{4})$/is;
	my $code="-$1";
	my $fname="Cities$code";
	$year=~/\d\d(\d\d)/is;
	my $ye=$1;
	$template=~s/<YEAR>/$ye/isg;
#	$jad=~s/<REGION>/$reg/isg;
	$template=~s/<CODE>/$code/isg;
#	$jad=~s/<DESC>/$desc/isg;
	$template=~s/<JAR>/$fname\.jar/isg;

	my $cmd=sprintf($amtools::unzip, '../source/template.zip', $srcdir);
	system($cmd);
	open(INF, ">$srcdir/META-INF/MANIFEST.MF") or die "No file";
		print INF $template;
	close(INF);
	my $stat="SELECT DISTINCT cities.name, data FROM cities, locations ".
		"WHERE cities.id IN ($ids) AND city_id=cities.id AND year=$year".
		" ORDER BY cities.name";
#	print $stat;
	my $sth = $dbh->prepare($stat);
	$sth->execute;
	my $i=0;
	while(my @row = $sth->fetchrow_array){
		push(@data, $row[1]);		
	}
	$sth->finish;
	amtools::join_datafiles2("$srcdir/locations.dat", \@data);
	$cmd=sprintf($amtools::zip, "../files/$fn", "$srcdir");
	system($cmd);
	my $asize= -s "../files/$fn.r";
	$template.="MIDlet-Jar-Size: $asize\n";
	open(FFF, ">../files/$fn.d");
	print(FFF $template);
	close(FFF);
	
	$template=~s%(MIDlet-Jar-URL: ).+?\n%$1http://astromaximum/cgi-bin/data\?r=$fn\n%is;
	open(FFF, ">../files/$fn.t");
	print(FFF $template);
	close(FFF);
	system("rm -R $srcdir/*");
	rmdir $srcdir;
	my $sql='INSERT INTO files (id, type, user_id, end_tm) VALUES';
	foreach (('r','d','t')){
		$sql.=" ($fn, \'$_\', ".$userid.", NOW()+ INTERVAL 2 HOUR),";
	}
	$sql=~s/,$//is;
	$sth = $dbh->prepare($sql);
	$sth->execute;
	$sth->finish;
	return $fn;
}