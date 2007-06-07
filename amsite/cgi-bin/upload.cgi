#!/usr/bin/perl

use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 1024;  # max 10K posts
use tools;
use amtools;

my $dsn = "DBI:mysql:database=amax;host=localhost";
my $dbh = DBI->connect($dsn, 'root', '');

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
  	my ($dir,$fn)=amtools::random('../inbox');
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
		print "<b>Reading $fn[0]</b><font size=-1><table border=1><tr><th>City</th><th>Country</th><th>Year</th><th>TXT</th><th>Cities DB</th></tr>";
		open FFF, "<$fn[0]";
  	@data=<FFF>;
  	close(FFF);
  	@fn=glob("$dir/Data*.dat");
  	
		foreach my $cc(@data){
			$cc=~s/^\"//isg;
			$cc=~s/\"\s*$//isg;
	  	my($name, $country, $yr, $txtchk, $status);
	  	$status=1;
			my @rec=split(/\|/is, $cc);
			$name=$rec[0];
			$name=~s/.+?\!//is;
			$country=$rec[3];
			$country=~s/.+?\$//is;
			open FF0, "<".shift(@fn);
	  	binmode(FF0);
	  	my $tr='';
	  	read(FF0,$yr,2);
	  	$yr=unpack("S",$yr);
	  	if($yr){
				seek(FF0,8,0);
				my $len=0;
				read(FF0,$len,2);
				$len=unpack("n",$len);
				read(FF0,$len,$len);
				close(FF0);
				if($len eq $name){
					$txtchk="<b>OK</b>";
				}
				else{
					$txtchk="<font color=red>doesn't match, found <b>$len</b></font>";
				}
			}
			else{
				$txtchk="<font color=red>missing</font>";
			}
			my $sth = $dbh->prepare("SELECT cities.id FROM cities WHERE SET used=\'t\' WHERE id=$dig AND type=\"$type\"");
			$sth->execute;
			$sth->finish;
			print "<tr><td>$name</td><td>$country</td><td>$yr</td><td>$txtchk</td><td>$status</td></tr>\n";
		}
		print "</table></font>";
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
my $fn="../files/".param($type).".$type";
if($type && -f $fn){
	my $sth = $dbh->prepare("UPDATE files SET used=\'t\' WHERE id=$dig AND type=\"$type\"");
	$sth->execute;
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