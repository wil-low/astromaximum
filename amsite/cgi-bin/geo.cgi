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

tools::cookie_check($dbh);

my $cnum=param('cid');
my $defyear=2007;

print header, start_html(-title=>'Astromaximum location archives', -script=>$ADDCITY);
print tools::adm_panel();

print start_form(-name=>'main', -method=>'post'), "<table width=100% border=1><tr valign=top><td><p><b>Year:</b> ",
popup_menu(-name=>'year', -values=>[qw(2005 2006 2007 2008)], -default=>$defyear), "</td>";
print "<td rowspan=2>", selected_cities(), "</td></tr>";
print "<tr><td>", country_header(), city_selector(), "</td>";
$dbh->disconnect;

print "</tr></table>";
print hidden(-name=>'sc', -default=>'.');
print hidden(-name=>'cid', -default=>'');
end_form();
#print Dump();
#print join('.',@sel_cities);

if(param('Action') eq 'Get data' && param('sc')=~/\d/){
	my ($fn,$id)=amtools::random('../files','.r');
	create_jar(param('year'), param('sc'), $fn);
	my $url='http://astromaximum/cgi-bin/data?r='.$id;
	print "<b><p align=center>JAR link: <a href=\'$url\'>$url</a><br><br>";
	$url=~s/\?r/\?d/is;
	print "JAD link: <a href=\'$url\'>$url</a><br>";
	print "<br>Attention: links are valid within next 2 hours!</p></b>";
}
print end_html;

sub country_header{
	my $res="<b>Country: </b>";
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
	$res.="<hr>";
	return $res;
}

sub city_selector{
	my $sth = $dbh->prepare(
		"SELECT cities.id, cities.name FROM cities,countries WHERE country_id=$cnum and countries.id=country_id ORDER BY cities.name");
	$sth->execute;
	my $res="<b>Cities:</b><br><div id=chkcit>";
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
	my $res="<h4 align=center>Selected cities:</h4><div id=selcit>".$rs."</div>";
#	if($rs){
		$res.="<div align=right>".button(-value=>'Delete selected', -onClick=>"city_del()")."</div>";
		$res.=submit('Action','Get data');
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

sub create_jar{ # $year, $city_ids, $outfile 
	my($year, $ids, $outfile)=@_;
	$ids=~/^\.(.+?)\.?$/is;
	$ids=$1;
	$ids=~s/\./\,/isg;
	my ($dir)=amtools::random('../source');
	my @data;
#	amtools::join_datafiles2("$dir/locations.dat", \@data);
#	system("zip --help >1.txt");
	mkdir $dir;
	system("unpack.bat $dir ../source/template.zip");
	my $stat="SELECT cities.id, cities.name, locations.data FROM cities,countries WHERE cities.id IN ($ids) and countries.id=country_id ORDER BY countries.name,cities.name";
#	print $stat;
#	my $sth = $dbh->prepare($stat);
#	$sth->execute;
#	while(my @row = $sth->fetchrow_array){
#		$res.="<input type=checkbox id=$row[0]></input>$row[1], $row[2]<br>\n";	
#	}
#	$sth->finish;
	open(FFF, ">$outfile");
	print(FFF $ids);
	close(FFF);
#	rmdir $dir;
}