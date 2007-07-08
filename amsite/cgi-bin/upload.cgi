#!/usr/bin/perl

use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 1024;  # max 10K posts
use tools;
use amtools;

	my $dbh = tools::db_connect();

tools::cookie_check($dbh);

	print header, start_html;
	my $fname=param('uploaded_file');
	error("Invalid archive: $fname") if $fname && $fname!~/\.(zip|rar)$/is;
	my $fh = upload('uploaded_file');
	if (!$fh && cgi_error) {
    print header(-status=>cgi_error);
    exit 0;
   }
	print tools::adm_panel();
	if($fh){
  	binmode($fh);
  	my @data=<$fh>;
  	close($fh);
  	my $d=join('',@data);
  	my ($dir,$fn)=amtools::random($tools::dir_inbox);
  	mkdir $dir;
  	my $arc="$dir/arc.zip";
  	open(FFF, ">$arc");
  	binmode(FFF);
  	print(FFF $d);
  	close(FFF);
		my $cmd=sprintf($amtools::unzip, $arc, $dir);
		system($cmd);
		unlink $arc;
		my @fn=glob("$dir/*.txt");
		error("TXT must be exactly one file in archive", $dir) if $#fn!=0;
		print "<b>Reading $fn[0]</b><font size=-1><table border=1><tr><th>City</th><th>Country</th><th>State</th><th>Year</th><th>TXT</th><th>Cities DB</th></tr>";
		open FFF, "<$fn[0]";
  	@data=<FFF>;
  	close(FFF);
  	@fn=glob("$dir/Data*.dat");
  	
		my $sthcou = $dbh->prepare(q(
			SELECT countries.id FROM countries WHERE countries.name=?)) || die $dbh->errstr;
		my $sthcit = $dbh->prepare(q(
			SELECT cities.id FROM cities,countries WHERE cities.name=? AND country_id=? AND state_id=?)) || die $dbh->errstr;
		my $sthstate = $dbh->prepare(q(
			SELECT states.id FROM states,countries WHERE states.name=? AND country_id=?)) || die $dbh->errstr;
		my $sthcouins = $dbh->prepare(q(
			INSERT INTO countries(name) VALUES (?))) || die $dbh->errstr;
		my $sthstateins = $dbh->prepare(q(
			INSERT INTO states(name,country_id) VALUES (?,?))) || die $dbh->errstr;
		my $sthcitins = $dbh->prepare(q(
			INSERT INTO cities(name,country_id,state_id) VALUES (?,?,?))) || die $dbh->errstr;
		my $sthloc = $dbh->prepare(q(
			SELECT id FROM locations WHERE year=? AND city_id=?)) || die $dbh->errstr;
		my $sthlocupd = $dbh->prepare(q(
			UPDATE locations SET data=? WHERE id=?)) || die $dbh->errstr;
		my $sthlocins = $dbh->prepare(q(
			INSERT INTO locations(year,city_id,data) VALUES(?,?,?))) || die $dbh->errstr;
			
		my($cou_count,$cit_count,$locins_count,$locupd_count,$state_count)=(0,0,0,0,0);	
		
		foreach my $cc(@data){
			$cc=~s/^\"//isg;
			$cc=~s/\"\s*$//isg;
			chomp($cc);
	  	my($name, $country, $yr, $txtchk, $status, $state);
	  	$status=0;
			my @rec=split(/\|/is, $cc);
			$name=$rec[0];
			$name=~s/.+?\!//is;
			$country=$rec[3];
			$country=~s/[\n\r]//isg;
			$country=~s/.+?\$//is;
			if($country=~s/ - (.+)//is){
				$state=$1;
			}
			my $curfn=shift(@fn);
			open FF0, "<$curfn";
	  	binmode(FF0);
	  	my $tr='';
	  	my $locdata='';
	  	read(FF0,$yr,2);
	  	$yr=unpack("S",$yr);
	  	if($yr){
				seek(FF0,8,0);
				my $len=0;
				read(FF0,$len,2);
				$len=unpack("n",$len);
				read(FF0,$len,$len);
				seek(FF0,0,0);
				read(FF0, $locdata, -s $curfn);
				close(FF0);
				my $tst=$name;
				if($state){
					$tst.=", $state";
				}
				if($len eq $tst){
					$txtchk="<b>OK</b>";
				}
				else{
					$txtchk="<font color=red>doesn't match, found <b>$len</b></font>";
				}
			}
			else{
				$txtchk="<font color=red>missing</font>";
			}
			my $couid=0;
			$sthcou->execute($country) || die $dbh->errstr;
			if(!$sthcou->rows){
				$sthcouins->execute($country) || die $dbh->errstr;
				$sthcou->execute($country) || die $dbh->errstr;
				$country="<font color=red>$country</font>";
				++$cou_count;
			}	
			($couid)=$sthcou->fetchrow_array;
			
			my $stateid=0;
			if($state){
				$sthstate->execute($state,$couid) || die $dbh->errstr;
				if(!$sthstate->rows){
					$sthstateins->execute($state,$couid) || die "$state>$couid>".$dbh->errstr;
					$sthstate->execute($state,$couid) || die $dbh->errstr;
					$state="<font color=red>$state</font>";
					++$state_count;
				}	
				($stateid)=$sthstate->fetchrow_array;
			}
			my $citid=0;
			$sthcit->execute($name,$couid,$stateid) || die $dbh->errstr;
			if(!$sthcit->rows){
				$sthcitins->execute($name,$couid,$stateid) || die $dbh->errstr;
				$sthcit->execute($name,$couid,$stateid) || die $dbh->errstr;
				$name="<font color=red>$name</font>";
				++$cit_count;
			}	
			($citid)=$sthcit->fetchrow_array;
			my $locid=0;
			$sthloc->execute($yr,$citid) || die $dbh->errstr;
			if($sthloc->rows){
				($locid)=$sthloc->fetchrow_array;
				$sthlocupd->execute($locdata, $locid) || die $dbh->errstr;
				++$locupd_count;
			}
			else{
				$sthlocins->execute($yr,$citid,$locdata) || die $dbh->errstr;
				$yr="<font color=red>$yr</font>";
				++$locins_count;
			}
#			my $sth = $dbh->prepare(
#				"SELECT cities.id, countries.id FROM cities,countries WHERE cities.country_id=countries.id ".
#				"AND cities.name=\"$name\" AND countries.name=\"$country\"");
			$status.="$citid, $couid, $stateid";
			print "<tr><td>$name</td><td>$country</td><td>$state</td><td>$yr</td><td>$txtchk</td><td>$status</td></tr>\n";
		}
		print "</table></font><p>Added <b>$cou_count</b> countries, <b>$state_count</b> states, <b>$cit_count</b> cities, <b>$locins_count</b> locations. Updated <b>$locupd_count</b> locations.";
  	print "<p><a href=upload.cgi>Back</a>";
  	amtools::rm_all($dir);
  }
  else{
		print start_multipart_form(-method=>'post');
		print filefield('uploaded_file','starting value',50,80);
		print "<br>",submit("Action", "Upload geodata");
		print end_form, Dump(), end_html;
	}
	exit;
	
	

my $type=undef;
$type='r' if defined(param('r'));
$type='d' if defined(param('d'));
$type='t' if defined(param('t'));
my $dig=param($type);
$dig=~/(\d{4})$/is;
my $idd=$1;
my $fn=$tools::dir_files.param($type).".$type";
if($type && -f $fn){
	my $sth = $dbh->prepare("UPDATE files SET used=\'t\' WHERE id=$dig AND type=\"$type\"") || die $dbh->errstr;
	$sth->execute || die $dbh->errstr;
	$sth->finish;
	$type='d' if $type eq 't';
	my $data;
	open(FFF, "<$fn");
	binmode(FFF);
	read(FFF, $data, -s $fn);
	close(FFF);
}
$dbh->disconnect;

sub error{ #message, dir to delete
	my ($msg,$dir)=(shift,shift);
	print "<h4>Error: ", $msg, "</h4>";
 	print "<p><a href=upload.cgi>Back</a>", end_html;
 	if(defined($dir)){
 		amtools::rm_all($dir);
 	}
	exit;
}
