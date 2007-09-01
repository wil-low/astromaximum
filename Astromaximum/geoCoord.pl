#!/usr/bin/perl
use strict;
use POSIX;
#use warnings;
use Encode;
use lib 'D:/Willow/prj/astrology/nomad_prj/'; 
use lib 'd:/projects/nomad_prj';
use tools;

if($#ARGV!=0 and $#ARGV!=1){
	die "Usage: <country group code> [year]\n";
}
my $city_inf=$ARGV[0]; # файл со списком городов

$0=~/(.+\/)/is;
our $mypath=$1;
our $path=$mypath."GeoAM/geo/";

my $country='';
my $tz;
my ($year, $day_count)=tools::get_year($mypath);
$year=$ARGV[1] if $ARGV[1];

my ($month, $day, $hour, $min)=(1,1,0,0);
my $tz_ofs=0;
{
#		warn $tm;
		my $tm=time;
		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = gmtime();
		my $tm2=POSIX::mktime($sec, $min, $hour, $mday, $m,$y,0,0,-1);
		
		$tz_ofs=$tm-$tm2;
}

my $sqpath='d:/projects/astro/v2/db/';
$sqpath='D:/Willow/prj/astrology/v2/db/' unless -d $sqpath;
$sqpath='../' unless -d $sqpath;

my $sqlite3='/usr/bin/';
$sqlite3=$sqpath unless -d $sqlite3;

$sqlite3.='sqlite3';
#die "$sqpath\n$sqlite3\n";

my %mon=qw(Jan 0 Feb 1 Mar 2 Apr 3 May 4 Jun 5 Jul 6 Aug 7 Sep 8 Oct 9 Nov 10 Dec 11);
my %wd=qw(Sun 0 Mon 1 Tue 2 Wed 3 Thu 4 Fri 5 Sat 6);


my $dir=$path."$city_inf";
mkdir $dir unless -d $dir;


our $outbuf;
our $fname;
my $InF=undef;
my $OutF;
my @cities;

	open($InF, "<$mypath".'city.inf') or die "No file $mypath".'city.inf';
	@cities=<$InF>;
	my $citlist=join("",@cities);
	$citlist=~s/\([^\)]+\)//isg;
	close($InF);
#	die $citlist;

	open($InF, "<$mypath".'timezone.inf') or die "No file";
	my @alltz=<$InF>;
	close($InF);
#	die $alltz[0];

#die get_tz('USA - District of Columbia', 'Washington');
#=head
if(! -f "$dir/$city_inf.txt"){
	my $error=0;
  unlink "$dir/$city_inf.txt";
	open($InF, "<$path$city_inf.ini") or die "No file $path$city_inf\.ini";
	@cities=<$InF>;
	close($InF);
	my $cid=1;
	$country='';
	my $state=0;
	my $invoke;
	my $db="$sqlite3 $sqpath".'coords.sqb';
	my $tmp=$path.'country.tmp';
	foreach my $cit(@cities){
		$cit=~s/[\n\r]//isg;
		next if $cit=~/\A\s*\Z/is;
		next if $cit=~/\#/is;
		if($cit=~s/\@\s*//is){
	#		die if $country;
			$country=$cit;
			$cit=~s/(\s-|\,).+//is;
			$cit=~s/\(.+?\)//isg;
			$cit=~s/[\-\d\+]//isg;
			my $sql="select id, eng from countries where eng = \'$cit\';";
			$invoke="echo \"$sql\" \| $db > \"$tmp\"";
			system($invoke);
			open($InF, "<$tmp") or die "No file $tmp";
			my @countries=<$InF>;
			close($InF);
			if($countries[0]=~/(\d+)\|/is){
				$cid=$1;
#				print "$country  capital =";
				$cit=$country;
				$cit=~s/\(.+?\)//isg;
				$cit=~s/.+,\s*//is;
				$cit=~s/\A\s+//is;
				$cit=~s/\s+\Z//is;
				print "$cit\n";
				next;
			}
			else{
				$cid=0;
				warn "*****Not found:  $country\n";
				$invoke="echo \"$cit\" >> \"$dir/$city_inf.txt\"";
		#		print "$invoke\n";
				system($invoke);
				next;
			}
		}
		next if $cit=~/\#/is;
		$cit=~s/\'/\'\'/isg;
		$cit=~s/(\!.+)//is;
		my $altcit=$1;
		my $state=$country;
		my $sql;
		if($state=~/USA \- (.+)/is){
			my $st_name=$1;
			if($state=~/(.+?)\*(.+)/is){
				$st_name=$2;
				$state=$1;
			}
			else{
				$st_name=~/(.+),/is;
				$st_name=$1;
			}
#			die $st_name;
			$state=~/(.+),/is;
			$state=$1;
			$sql="select cities.eng, longit, latit, \'$state\' from cities,counties where cities.eng = \'$cit\' and country_id=$cid and county_id=counties.id and counties.eng=\'$st_name\'";
			print "$sql\n";
		}
		else{	
			$state=~/(.+),/is;
			$state=$1;
			$sql="select eng, longit, latit, \'$state\' from cities where eng = \'$cit\' and country_id=$cid limit 1";
		}
		$invoke="echo \"$sql;\" \| $db > \"$tmp\"";
#		print "$invoke\n";
		system($invoke);
		open($InF, "<$tmp") or die "No file $tmp";
		my @countries=<$InF>;
		close($InF);
		if($countries[0]=~/\|/is){
			chomp($countries[0]);
			$countries[0]=~s/\|/$altcit\|/is;
			my @params=split(/\|/is, $countries[0]);
			$params[0]=~s/.+!//is;
			$error++ if !get_tz($params[3],$params[0],0);
			$invoke="echo \"$countries[0]\" >> \"$dir/$city_inf.txt\"";
#			print "$invoke\n";
			system($invoke);
		}
		else{
			warn "*** $cit ($state) not found\n";
			$error++;
		}
		
	}
  unlink $tmp;
  if($error){
  	unlink "$dir/$city_inf.txt";
  	die "Please correct $error errors.\n";
  }
  else{
		warn "Ready. Check coords.\n";
	}
	<STDIN>;
}
#####################################
	open($InF, "<$dir/$city_inf.txt") or die "No file";
	@cities=<$InF>;
	close($InF);
#	die "@cities";
	my $i=0;
	our $city;
	undef $/ ;
	my $newdir=sprintf('mutter/output/archive/%d/%s',$year,$city_inf);
	mkdir $newdir unless -d $newdir;
#	foreach my $cit(@cities){
#		chomp($cit);
#		next if $cit=~/\A\s*\Z/is;
#		$cit=~s/\A\s*\"(.+)\"\s*\Z/$1/is;
#		next if $cit=~/\#/is;
#		next if $cit!~/\d/is;
#		my @params=split(/\|/is, $cit);
#		if(! -f $fname){
#			$city=$params[0];
#			$city=~s/.+!//is;
#			get_tz($params[3],$city);
#		}		
#	}
	foreach my $cit(@cities){
		$outbuf='';
		chomp($cit);
		$cit=~s/\A\s*\"(.+)\"\s*\Z/$1/is;
		next if $cit=~/\A\s*\Z/is;
		next if $cit=~/\#/is;
		next if $cit!~/\d/is;
		my @params=split(/\|/is, $cit);
		$fname=$newdir.sprintf('/Data%02d.dat',$i);
		$city=$params[0];
		$city=~s/.+!//is;
		if(! -f $fname){
			print "\n-----------------------";
			print "\n******** $city ********";
			print "\n-----------------------\n";
			my $tz=get_tz($params[3],$city,1);
			my $dstbuf=calc_dst($tz);
			my $invoke=$mypath."mutter2/mutter2 $year geo0- $params[1] $params[2]";# electio";
			print "$invoke\n";
			system($invoke);
			if($params[3]=~/USA \- (.+)/is){
				$city.=", $1";
			}
			$city=~s/[\n\r]//isg;
			writeUTF($city);
			
			my $header=pack('SCCCCSa*a*',$year, $month, $day, $hour, $min, $day_count, $outbuf, $dstbuf);
			
			print "$fname\n";
			open($OutF, ">$fname") or die "No file";
			binmode($OutF);
			print $OutF $header;
			close($OutF);
			
			my $geomask=sprintf('mutter/output/archive/%d/geo0-*.bin',$year);
			my @bins=glob($geomask);
			die "No files: $geomask" if $#bins<0;
			my $counter=0;
			print join(@bins,"\n");
			foreach my $ff(@bins){
				tools::writeData($ff, $fname, 0);
			}	
		}
		else{
			data_check($fname, $year, $city);
		}

		$i++;
	}
	open($InF, ">$newdir/$city_inf.txt");
	print($InF join('', @cities));
	close($InF);
#	my @bins=glob("$dir\\Data*.dat");
#	tools::join_datafiles($i, "$dir\\locations.dat", \@bins);

sub get_tz{
	my ($country,$city,$isdie)=@_;
	$country=~s/.+\$//is;
	$country=~s/[\n\r]//isg;
	if($citlist=~/\@ ($country[^\@]+?$city(?: \([^\n\r]+\))?)/is){
		my $tzz=$1;
		$tzz=~/\A(.+?)\n/is;
		$tzz=$1;
		$tzz=~s/(\W)/\\$1/isg;
		foreach my $li(@alltz){
			return $li if $li=~/$tzz/is;
		}
	}
	if($isdie){
		die "No TZ for $country, $city!";
	}
	else{
		warn "No TZ for $country, $city!\n";
	}
	return undef;
}

sub calc_dst{
	my $buf;
	my @fld=$_[0]=~/([\d\+\-\.]+)\s+(?:(\S+\@\S+)\s+(\S+\@\S+)\s+)?(.+)/is;
	print join('|',@fld)."\n";
#	die "\n$#fld";
#	die "Invalid TZ: $_[0]\n" if !$fld[1] || $fld[2];
	my $ofs=$fld[0]*60;
	print "TZ offset=$ofs\n"; #in mins
	$ofs+=(16*60);
	if($fld[1] && $fld[2]){
		print 'Start ';
		my $start_dst=decode_time($fld[1]);

		print 'End ';
		my $end_dst=decode_time($fld[2]);
		print "$start_dst,$end_dst\n";
		$buf=pack('NN',$start_dst,$end_dst);
	}
	else{
		$ofs+=(1<<15);
	}
	$buf=pack('n',$ofs).$buf;
#	die;
	return $buf;
}

sub decode_time{
	my $tm=undef;
	print "$_[0]:\n";
	$_[0]=~s/\@([\d\+\-\.]+)/\@/is;
	my $hr_frac=$1*3600;
	my $hr=int($hr_frac/3600);
	$hr_frac-=$hr*3600;
	my $mn=int($hr_frac/60);
	$hr_frac-=$mn*60;
	my $sc=$hr_frac;
#	die "$hr $mn $sc";
	if($_[0]=~/(\d+)(\w{3})\@/is){ # 01Apr@3
		$tm=POSIX::mktime($sc, $mn, $hr, $1, $mon{$2},$year-1900,0,0,-1);
#		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = gmtime($tm);
#		die $wday;
	}
	elsif($_[0]=~/last/is){ # LastSunMar@2
		my @pp=$_[0]=~/last(\w{3})(\w{3})\@/is;
		my $month=$mon{$pp[1]}+1; # next month
#		print $mon{$pp[1]};
		$tm=POSIX::mktime($sc, $mn, $hr, 0, $month,$year-1900,0,0,-1);
		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime($tm);
		$pp[0]=$wd{$pp[0]};
#		print "last $wday, need $pp[0]\n";
		$wday=($wday+7-$pp[0])%7;
		$tm=POSIX::mktime($sec,$min,$hour,$mday-$wday,$m,$y,0,0,-1);
#		die $wday;
#		$tm=POSIX::mktime(0, 0, 0, 10, 1,2007-1900,0,0,-1);#-$tz_ofs;
	}
	if($_[0]=~s/first(\w{3})//is){ 
		my($after,$week_day,$m)=(1,$wd{$1});
		if($_[0]=~/after(\d+)(\w{3})\@/is){ # FirstSunAfter18Mar@2
			($after,$m)=($1+1,$mon{$2},$3);
		}
		else{
			$_[0]=~/(\w{3})\@/is; # FirstSunMar@2
			$m=$mon{$1};
		}
		$tm=POSIX::mktime($sc, $mn, $hr, $after, $m,$year-1900,0,0,-1);
		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime($tm);
#		print "$after,need $week_day,$m,$hr\n";
#		die $wday;
		$wday=(7+$week_day-$wday)%7;
		$tm=POSIX::mktime($sec,$min,$hour,$mday+$wday,$m,$y,0,0,-1);
#		$tm=POSIX::mktime(0, 0, 0, 10, 1,2007-1900,0,0,-1);#-$tz_ofs;
		
	}
  print "Date = ", POSIX::ctime($tm);
	return ($tm+$tz_ofs)/60;
}

sub writeUTF
{
	my $param=shift;
	print "$param\n";
	$param = decode("cp1251", $param);
	my $len=0;
	{
		use bytes; $len=length($param);
	}
	$outbuf.=pack('na*', $len, $param);
#	$outbuf.=$param;
#	die $outbuf;
}

sub data_check
{
	#in: fname, year, cityname
	my ($data_year, $dc_len, $data_city);
	open(FILE, "<$_[0]");
	read(FILE, $data_year,4);
	$data_year=unpack('S',$data_year);
	seek(FILE,8,0);
	read(FILE, $dc_len,2);
	$dc_len=unpack('n',$dc_len);
	read(FILE, $data_city,$dc_len);
	close(FILE);
	if($data_year==$_[1] and $data_city eq $_[2]){
		return 1;
	}
	else{
		die "$_[0] contains \"$data_city\" and \"$data_year\"!\n";
	}
}
	
