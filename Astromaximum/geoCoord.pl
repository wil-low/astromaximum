#!/usr/bin/perl
use strict;
use POSIX;
#use warnings;
#use Encode;
our $winda=$^O=~/Win/is;

my $TZ_VER=2;

my %mon=qw(Jan 0 Feb 1 Mar 2 Apr 3 May 4 Jun 5 Jul 6 Aug 7 Sep 8 Oct 9 Nov 10 Dec 11);
my %wd=qw(Sun 0 Mon 1 Tue 2 Wed 3 Thu 4 Fri 5 Sat 6);
my($tzonly, $clean, $fnfix)=(0,0,0);

$0=~/(.+[\/\\])/is;
our $mypath=$1;
$mypath='./' unless $mypath;
require $mypath.'tz_patches.pm';

my $year=shift(@ARGV);
if($ARGV[0] eq 'tzonly'){
	$tzonly=1;
	shift(@ARGV);
}
if($ARGV[0] eq 'clean'){
	$clean=1;
	shift(@ARGV);
}
if($ARGV[0] eq 'fnfix'){
	$fnfix=1;
	shift(@ARGV);
}
if($#ARGV!=0 and scalar(@ARGV)<2){
	die "Usage: <year> [tzonly|clean|fnfix] <country group code list>|<all>|<common>\n";
}
our %historic;
my @cities;
my $citlist;
my @alltz;
if($TZ_VER==2){
	open(InF, "<$mypath".'data/tz/city.txt') or die "No file $mypath".'data/tz/city.txt';
	@cities=<InF>;
	close(InF);
	open(InF, "<$mypath".'data/tz/city_add.txt') or die "No file $mypath".'data/tz/city_add.txt';
	@alltz=<InF>;
	close(InF);
	push(@cities, @alltz);
	undef @alltz;
	foreach (@cities){
		$_=~s/[\_\^].+/\n/is;
	}
	$citlist=join("",@cities);
	$citlist=~s/\([^\)]+\)//isg;
	process_historic();
}
else{
	open(InF, "<$mypath".'timezone.inf') or die "No file";
	@alltz=<InF>;
	close(InF);
	open(InF, "<$mypath".'city.inf') or die "No file $mypath".'city.inf';
	@cities=<InF>;
	close(InF);
	$citlist=join("",@cities);
	$citlist=~s/\([^\)]+\)//isg;
}


undef @cities;

require $mypath.'tools.pm';
do_patch(\$citlist);

my $day_count=tools::day_count($year);
mkdir $mypath."data/archive";
mkdir $mypath."data/ephdata";
our $path=$mypath."data/archive/";
our $city_inf;
my $country='';
my $tz;

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

our $outbuf;
our $fname;

if($ARGV[0] eq 'all'){
	print "Making all...\n";
	my @ini=glob($mypath."data/*.ini");
	foreach $city_inf(@ini){
		$city_inf=~/.+[\/\\](.+?)\.ini/is;
		$city_inf=$1;
		print "---- $city_inf ----";
		if($fnfix){
			my $cnt=0;
			foreach my $fn(glob($path."$year/$city_inf/Data*.dat")){
				my $newfn=$fn;
				$newfn=~s/Data(\d+)\.dat$//s;
				$newfn.=sprintf("Data%04d.dat", $1);
				if($fn ne $newfn){
					rename($fn, $newfn);
					$cnt++;
				}
			}
			print "\trenamed: $cnt\n";
		}
		else{
			process_ini();
		}
	}
	exit(0);
}
if($ARGV[0] eq 'common'){
	print "Making common...\n";
	my $invoke=$mypath."mutter2/mutter2 $year";# electio";
	print "$invoke\n";
	my $res=system($invoke);
	die "Cancelled, result=$res" if $res;
	exit(0);
}
else{
	foreach $city_inf(@ARGV){
		print "---- $city_inf ----";
		process_ini();
	}
}

sub process_ini{
	print "\n";
	if($clean){
		unlink "$path$city_inf.txt";
	}
	if(! -f "$path$city_inf.txt"){
		my $error=0;
		unlink "$path$city_inf.txt";
		open(InF, "<$mypath"."data/$city_inf.ini") or die "No file $path"."data/$city_inf\.ini";
		@cities=<InF>;
		close(InF);
		my $cid=1;
		$country='';
		my $state=0;
		my $invoke;
		my $db="$sqlite3 $sqpath".'coords.sqb';
		my $tmp=$path.'country.tmp';
		my $contin='';
		foreach my $cit(@cities){
			$cit=~s/[\n\r]//isg;
			next if $cit=~/\A\s*\Z/is;
			next if $cit=~/\#/is;
			if($cit=~s/&([A-Z]{3})//is){
			  $contin=$1;
			  die "Invalid continent: $contin" unless $contin=~/^(AFR|ASI|EAS|SAS|SEA|CAR|CAM|EUR|EEU|WEE|MIE|NAM|OCE|SAM)$/is;
			  next;
			}
			if(!$contin){
			  die "No continent in $cit";
			}
			if($cit=~s/\@\s*//is){
		#		die if $country;
				$country=$cit;
				$country=~s/.+\$//is;
				if($cit=~s/(.+)\$//is){
				    $cit=$1;
				}
				$cit=~s/(\s-|\,).+//is;
				$cit=~s/\(.+?\)//isg;
				$cit=~s/[\-\d\+]//isg;
				$cit=~s/^\s+//is;
				$cit=~s/\s+$//is;
				my $sql="select id, eng from countries where eng = \'$cit\';";
				$invoke="echo \"$sql\" \| $db > \"$tmp\"";
				system($invoke);
				open(InF, "<$tmp") or die "No file $tmp";
				my @countries=<InF>;
				close(InF);
				if($countries[0]=~/(\d+)\|/is){
					$cid=$1;
	#				print "$country  capital =";
					$cit=$country;
					$cit=~s/\(.+?\)//isg;
					$cit=~s/.+,\s*//is;
					$cit=~s/\A\s+//is;
					$cit=~s/\s+\Z//is;
#					print "$cit\n";
					next;
				}
				else{
					$cid=0;
					print "$invoke\n";
					warn "\n*****Country not found:  $country";
					$invoke="echo \"$cit\" >> \"$path$city_inf.txt\"";
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
				$sql="select cities.eng, longit, latit, '$state','$contin' from cities,counties where cities.eng = '$cit' and country_id=$cid and county_id=counties.id and counties.eng=\'$st_name\'";
#				print "$sql\n";
			}
			else{	
				$state=~/(.+),/is;
				$state=$1;
				$sql="select eng, longit, latit, \'$state\','$contin' from cities where eng = \'$cit\' and country_id=$cid limit 1";
			}
			$invoke="echo \"$sql;\" \| $db > \"$tmp\"";
			system($invoke);
			open(InF, "<$tmp") or die "No file $tmp";
			my @countries=<InF>;
			close(InF);
			if($countries[0]=~/\|/is){
				chomp($countries[0]);
				$countries[0]=~s/\|/$altcit\|/is;
				$countries[0]=~s/\'\'/\'/isg;
				my @params=split(/\|/is, $countries[0]);

				$params[0]=~s/.+!//is;
				$error++ if !get_tz($params[3],$params[0],0,0);
				$invoke="echo \"$countries[0]\" >> \"$path$city_inf.txt\"";
	#			print "$invoke\n";
				system($invoke);
			}
			else{
				print "$invoke\n";
				warn "*** $cit ($state) not found in Janus DB";
				$error++;
			}

		}
	  unlink $tmp;
	  if($error){
		unlink "$path$city_inf.txt";
		die "Please correct $error errors.\n";
	  }
	  else{
			print "Ready. Check coords.\nMay I continue calculations (y/n)? ";
		}
		my $ans=<STDIN>;
#		print ">$ans<";
		chomp($ans);
		die "Calculation cancelled.\n" unless $ans eq 'y';
	}
	#####################################
		open(InF, "<$path$city_inf.txt") or die "No file";
		@cities=<InF>;
		close(InF);
	#	die "@cities";
		my $i=0;
		my $hrepl=0;
		our $city;
	#	undef $/ ;
		my $newdir=ensure_slash(sprintf('%sdata/archive/%d/%s',$mypath,$year,$city_inf));
		my $arcdir=$mypath.'data';
		if(!-d $newdir){
			mkdir $newdir or die $!;
		}
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
			$fname=$newdir.sprintf('/Data%04d.dat',$i);
=head
			my $newfn=$fname;
			if($newfn=~s/Data(\d\d)\.dat//is){
				$newfn.=sprintf("%04d",$1);
				rename($fname, $newfn) if -f $fname;
				$fname=$newfn;
			}	
			die $fname;
=cut
			$city=$params[0];
			$city=~s/.+!//is;
			if(! -f $fname or $tzonly){
				print "\n******** $city ********\n";
				my $tz=get_tz($params[3],$city,0,0);
				my $dstbuf=calc_dst($tz);
				if($params[3]=~/USA \- (.+)/is){
					$city.=", $1";
				}
				$city=~s/[\n\r]//isg;
				writeUTF($city);

				my $header=pack('SCCCCSa*a*',$year, $month, $day, $hour, $min, $day_count, $outbuf, $dstbuf);
				if(!$tzonly){
					my $invoke=ensure_slash($mypath."mutter2/mutter2 $year geo0- $params[1] $params[2]");# electio";
					print "$invoke\n";
					my $res=system($invoke);
					die "Cancelled, result=$res" if $res;
					
	#				print "$fname\n";
					open(OutF, ">$fname") or die "$! $fname";
					binmode(OutF);
					print OutF $header;
					close(OutF);

					my $geomask=sprintf('%sdata/archive/%d/geo0-*.bin',$mypath, $year);
					my @bins=glob($geomask);
					die "No files: $geomask" if $#bins<0;
					my $counter=0;
					print join(@bins,"\n");
					foreach my $ff(@bins){
						tools::writeData($ff, $fname, 0);
					}	
				}
				else{
					$hrepl+=tz_check($fname, $header, "$year-$city");
				}
			}
			else{
				data_check($fname, $year, $city);
			}

			$i++;
		}
	#	if(!$tzonly){
			unlink "$arcdir/$year/$city_inf.zip";
#			if(-f "$arcdir/$year/$city_inf.zip"){
#				print "$year/$city_inf.zip exists. Please delete it to regenerate.\n";
#			}
#			else{
				open(InF, ">$newdir/$city_inf.txt");
				print(InF join("\n", @cities));
				close(InF);
				unlink("$newdir/$city_inf.zip");
				my $cmd;
				if($winda){
					$cmd=ensure_slash(sprintf('cd %s & ../../../zip  %s *.txt *.dat & cd ../../../../', $newdir, $city_inf));
				}
				else{
					$cmd=ensure_slash(sprintf('cd %s ; zip -q %s *.txt *.dat ; cd ../../../../', $newdir, $city_inf));
				}
				print "$cmd\n";
				system($cmd);
				mkdir(ensure_slash("$arcdir/$year"));
				rename(ensure_slash("$newdir/$city_inf.zip"), ensure_slash("$arcdir/$year/$city_inf.zip")) or die $!." $newdir/$city_inf.zip";
				print "Written $arcdir/$year/$city_inf.zip\n";
			#	my @bins=glob("$dir\\Data*.dat");
			#	tools::join_datafiles($i, "$dir\\locations.dat", \@bins);
	#		}
#		}
		print "\nHeaders replaced: $hrepl.\n" if $tzonly;
}

sub calc_dst{
	my $buf;
	my @fld=$_[0]=~/([\d\+\-\.]+)\s+(?:(\S+\@\S+)\s+(\S+\@\S+)\s+)?(.+)/is;
	print join('|',@fld).",\t";
#	die "\n$#fld";
#	die "Invalid TZ: $_[0]\n" if !$fld[1] || $fld[2];
	my $ofs=$fld[0]*60;
	print "TZ offset=$ofs\n"; #in mins
	$ofs+=(16*60);
	if($fld[1] && $fld[2]){
		print 'Start ';
		my $start_dst=decode_time($year, $fld[1]);

		print 'End ';
		my $end_dst=decode_time($year, $fld[2]);
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

sub ctime2number{
	$_[0]=~/\w{3} (\w{3}) (\d+) (\d\d):(\d\d):(\d\d) (\d{4})/is;
	return sprintf("%04d%02d%02d%02d%02d%02d", $6, $mon{$1}+1, $2, $3, $4, $5);
}

sub tz_check{
	my($fname, $header, $comment)=@_;
	my $hlen=length($header);
	open(InF1, "<$fname") or die "$! $fname";
	binmode(InF1);
	my @data=<InF1>;
	close(InF1);
	my $body=join('', @data);
#	my $citylen=unpack("n",substr($body,8,2));
#	die "$citylen, $comment, $fname";
	$body=~/^(.{$hlen})/s;
	my $oldhdr=$1;
	if($oldhdr ne $header){
		warn("*** $comment: Replaced header in $fname");
		print "old: ".unpack("H*",$oldhdr)."\nnew: ".unpack("H*",$header)."\n";
		$body=~s/^.{$hlen}/$header/s;
		open(OutF, ">$fname") or die "$! $fname";
		binmode(OutF);
		print OutF $body;
		close(OutF);
		return 1;
	}
	return 0;
}

sub decode_time{
	my $tm=undef;
	my ($year, $str)=@_;
	print "$str: ";
	$str=~s/\@([\d\+\-\.]+)/\@/is;
	my $hr_frac=$1*3600;
	my $hr=int($hr_frac/3600);
	$hr_frac-=$hr*3600;
	my $mn=int($hr_frac/60);
	$hr_frac-=$mn*60;
	my $sc=$hr_frac;
#	die "$hr $mn $sc";
	if($str=~/(\d+)(\w{3})\@/is){ # 01Apr@3
		$tm=POSIX::mktime($sc, $mn, $hr, $1, $mon{$2},$year-1900,0,0,-1);
#		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = gmtime($tm);
#		die $wday;
	}
	elsif($str=~/last/is){ # LastSunMar@2
		my @pp=$str=~/last(\w{3})(\w{3})\@/is;
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
	if($str=~s/first(\w{3})//is){ 
		my($after,$week_day,$m)=(1,$wd{$1});
		if($str=~/after(\d+)(\w{3})\@/is){ # FirstSunAfter18Mar@2
			($after,$m)=($1+1,$mon{$2},$3);
		}
		else{
			$str=~/(\w{3})\@/is; # FirstSunMar@2
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
	my $tmstr=POSIX::ctime($tm);
	print "\t$tmstr";
	$tm=($tm+$tz_ofs)/60;
	if(wantarray){
		return ($tm, $tmstr);
	}
	else{
		return $tm;
	}
}

sub writeUTF
{
	my $param=shift;
#	print "$param\n";
#	$param = decode("cp1251", $param);
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
	my ($data_year, $dc_len, $data_city, $body);
#	print " data_check(".join(',', @_).")\n";
	open(FILE, "<$_[0]");
	binmode(FILE);
	read(FILE, $data_year,4);
	$data_year=unpack('S',$data_year);
	seek(FILE,8,0);
	read(FILE, $dc_len,2);
	$dc_len=unpack('n',$dc_len);
	read(FILE, $data_city,$dc_len);
	seek(FILE, 0, 0);
	read(FILE, $body, 100000);
	close(FILE);
	if($data_year!=$_[1]){
		print "$_[0] contains \"$data_year\" instead of $_[1]!\n";
		die;
	}
	$data_city=~s/,.+//is;
#	print "\t$data_city\n";
	if($data_city ne $_[2]){
		print "$_[0] contains \"$data_city\" ";
		print "instead of \"$_[2]\"! Replace (y/n)?\n";
		my $ans=<STDIN>;
		chomp($ans);
		die if $ans ne 'y';
		$dc_len=pack('n', length($_[2]));
		$body=~s/..$data_city/$dc_len$_[2]/s;
		open(FILE, ">$_[0]") or die $!;
		binmode(FILE);
		print FILE $body;
		close(FILE);
	}
	return 1;
}

sub process_historic
{
use warnings;
#   data structure
#	$historic={
#		'-<RULE>'=>[
#			{
#				year=>YEAR_PERIOD,
#				start=>DST_START,
#				end=>DST_END,
#				diff=>DST_DIFF_IN_HOURS
#			}
#		],
#		'<COUNTRY>'=>[
#			{
#				ofs=>DST_OFFSET,
#				rule=>RULE_OF_PERIOD,
#				end_date=>END_OF_PERIOD,
#			}
#		]
#	}

	open(HIST, "<data/tz/Historic.txt");
	my $secflag=0;
	my $secname=''; # section header
	print "Historic.txt: ";
	while(my $ln=<HIST>){
		do_patch(\$ln);
		$ln=~s/[\n\r]//isg;
		$ln=~s/\#.+//is; # strip comments
		if($ln=~/^\s*$/is){ # empty line, section end
			$secflag=0;
			next;
		}
		if(!$secflag){
			if($ln=~s/^Rule //is){
				$secflag=1; # rule begins
				$secname="-$ln";
#				print "R";
#				$historic{$secname}="[0,1,2]";
			}
			else{
				my @linkto=split(/\s*LinkTo\s*/, $ln);
				if(scalar(@linkto)==2){ # LinkTo clause
					$historic{$linkto[0]}->{ofs}=">$linkto[1]"; #linking
#					print "L";
				}
				else{
					$secflag=2; # country begins
#					$ln=~s/(.+), .+/$1/is; # remove trailing capital
					$secname=$ln; # no link, section continues
					$secname=~s/\(.+//is; # remove alternate capital name
					$secname=~s/\s+$//is;
					$secname=~s/^\s+//is;
#					print ">$secname<\n";
#					print "C";
				}
			}
			next;
		}	
		if($secflag==1){ # parsing rule section
			my @row=split(/\t/, $ln);
			if(scalar(@row)!=3 and scalar(@row)!=4){
				die "Wrong format: $ln in rule $secname";
			}
			my $diff=1;
			$diff=$row[3] if defined($row[3]);
			push(@{$historic{$secname}}, 
				{year=>$row[0], start=>$row[1], end=>$row[2], diff=>$diff});
#			last;
		}
		if($secflag==2){ # parsing country section
			my @row=split(/\t/, $ln);
			if(scalar(@row)!=3){
				die "Wrong format: $ln in country $secname";
			}
			push(@{$historic{$secname}}, 
				{ofs=>$row[0], rule=>$row[1], end_date=>$row[2]});
#			last;
		}
	}
	close(HIST);
	print " Ready.\n";
#	die $historic{'Yemen, Sana�a'};
}
	
sub get_tz{
	my ($country,$city,$isdie,$verbose)=@_;
	$country=~s/.+\$//is;
	$country=~s/[\n\r]//isg;
	if($TZ_VER==2){
		my $c_arr;
		print "$country,$city,$isdie\t" if $verbose;
		if($citlist=~/\@ (\Q$country\E[^\@]+?$city(?: \([^\n\r]+\))?)/is){
			$country=$1;
			if($country=~/\A(.+?)\s*\n/is){
			  $country=$1;
			}
			$c_arr=$historic{$country}; # TZ hash
#			die "No TZ for $country!" unless defined $c_arr;
		}
		else{
		  warn "No TZ for $country,$city!";
                  return undef;
		}
		if(!defined $c_arr){
#			die $citlist;
#			print join("<\n", sort(keys(%historic)));
			if($isdie){
				unlink "$path$city_inf.txt";
				die "No TZ for $country, $city!";
			}
			else{
				warn "No TZ for $country, $city!";
				return undef;
			}
		}
		my @result;
		my($ofs, $start, $end, $diff)=(0, 0, 0, 1);
		if(ref($c_arr) eq 'HASH' and $c_arr->{ofs}=~/^>(.+)/is){ # LinkTo redirect
			$country=$1;
			print "LinkTo $country\n" if $verbose;
			$c_arr=$historic{$country};
			die "No TZ for $country!" unless defined $c_arr;
		}
		foreach my $row(@$c_arr){
			if($row->{end_date}=~/(\d{4})/is){ # end year
				$end=$1;
			}
			else{
				$end=9999; # max
			}
#			print "\t - $start $end\n" if $verbose;
			if($year>=$start and $year<$end){ # we're inside period
				$ofs=$row->{ofs};
				my $rule=$row->{rule};
				if($rule eq '-'){ #no rule
					print "-\n" if $verbose;
					if($year==$end){ #year exactly at period's end
						$end=$row->{end_date};
					}
					else{
						$end=$year;
					}
					$start=$end=$diff='';
				}
				else{
					print "Rule $rule\n" if $verbose;
					my $r_arr=$historic{-$rule}; # follow rule
					die "No rule for $rule!" unless defined $r_arr;
					$start=0;
					foreach my $rulerow(@$r_arr){
						my $period=$rulerow->{year};
						my ($y0, $y1)=split(/-/, $period); # year range
						$y1=9999 if $y1 eq 'max';
						$y1=$y0 unless $y1;
						if($year>=$y0 and $year<=$y1){ # we're inside period
							$start=$rulerow->{start};
							$end=$rulerow->{end};
							$diff=$rulerow->{diff};
							last;
						}
						$start=$y1; # probe next period
					}
				}
				last;
			}
			$start=$end; # probe next period
		}
		if($start==9999){
			print "Cannot handle - too complicated\n";
		}
		$start=~s/(\d+)\(UTC\)/$1+$ofs/e;
		$end=~s/(\d+)\(UTC\)/$1+$ofs+$diff/e;
		my @oo=split(/:/, $ofs);
		$ofs=($oo[0]*3600+$oo[1]*60+$oo[2])/3600;
#		die $ofs;
		if(wantarray()){
			push(@result, "$ofs, $start, $end, $diff");
			return @result;
		}
		else{
			return "$ofs\t$start\t$end\t$country";
		}
	}
	else{
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
			warn "No TZ for $country, $city!";
		}
		return undef;
	}
}

sub ensure_slash{
	$_[0]=~s/\//\\/isg if $winda;
	return $_[0];
}
