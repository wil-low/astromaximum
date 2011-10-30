#!/usr/bin/perl
use strict;
use POSIX;
use warnings;
use Data::Dumper;

our $winda=$^O=~/Win/is;

my $TIMEZONE_DIR = '/usr/share/zoneinfo';

my $TZ_VER=2;

my %mon=qw(Jan 0 Feb 1 Mar 2 Apr 3 May 4 Jun 5 Jul 6 Aug 7 Sep 8 Oct 9 Nov 10 Dec 11);
my %wd=qw(Sun 0 Mon 1 Tue 2 Wed 3 Thu 4 Fri 5 Sat 6);
my($tzonly, $clean)=(0,0);

my %zoneinfo; # cumulative hash to hold zoneinfo, filled in get_tz_array()

our $mypath='';
if ($0=~/(.+[\/\\])/is) {
	$mypath = $1;
}
$mypath='./' unless $mypath;

my $epoch=jul(1970, 1, 1, 0);

our $city_info;

my $year=shift(@ARGV);
if($ARGV[0] eq 'tzonly'){
	$tzonly=1;
	shift(@ARGV);
}
if($ARGV[0] eq 'clean'){
	$clean=1;
	shift(@ARGV);
}

if($#ARGV!=0 and scalar(@ARGV)<2){
	die <<EOF;
Astrological events calculator
Usage: 
    geoCoord.pl <year> [tzonly|clean|fnfix|mod] <module>|common|<country group code list>|all
Options:
    tzonly - only update timezone dates, no calculation
    clean  - regenerate city coords
    fnfix  - rename files to new convention (deprecated)
    mod    - do not calculate, use <module> to modify existing data
        syntax: amax_geo::custom(\$hdr, \$custom, \$geo)

    common - calculate common.dat for this year
    all    - calculate all cities
EOF
}

require $mypath.'tools.pm';

my $day_count=tools::day_count($year);
mkdir $mypath."data/archive";
mkdir $mypath."data/archive/$year";
mkdir $mypath."data/ephdata";
our $path=$mypath."data/archive/";
our $city_inf;
our $hrepl=0;
my $country='';
my $tz;

my ($month, $day, $hour, $min)=(1,1,0,0);
my $tz_ofs=0;
{
	my $tm=time;
	my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = gmtime();
	my $tm2=POSIX::mktime($sec, $min, $hour, $mday, $m,$y,0,0,-1);
	
	$tz_ofs=$tm-$tm2;
}

###### TZ testing
#=head
{
	$ENV{TZ} = 'GMT';
	POSIX::tzset();
	my ($std, $dst) = POSIX::tzname();
	print "($std, $dst)";
	# 954032400 > Sun Mar 26 01:00:00 2000 = name: EEST gmt_ofs_sec: 10800 is_dst: 1
	die POSIX::mktime(0, 0, 1, 26, 2, 2000 - 1900, 0, 0, -1);
	my $tz=dump_zone('Europe/Kiev');
	#my $tz=get_tz_array(1943, 'America/New_York');
	#print Data::Dumper->Dump([$tz], [qw($tz)]);
	exit;
}
#=cut
######

my $sqpath='d:/projects/astro/v2/db/';
$sqpath='D:/Willow/prj/astrology/v2/db/' unless -d $sqpath;
$sqpath='../' unless -d $sqpath;

die "coords.sqb not found at $sqpath" unless -f $sqpath."coords.sqb";

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
		process_ini();
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
		process_ini($city_inf);
	}
}

sub process_ini{
	my ($city_inf) = @_;
	print "---- $city_inf ----\n";
	if($clean){
		unlink "$path$city_inf.txt";
	}
	if(! -f "$path$city_inf.txt"){
		my $error=0;
		open(InF, "<$mypath"."data/$city_inf.ini") or die "No file $path"."data/$city_inf\.ini";
		my @cities=<InF>;
		close(InF);
		open(CITY_INF, ">$path$city_inf.txt") or die "No file $path$path$city_inf.txt";
		# write cities info + tz info
		foreach my $city(@cities) {
			my $tz_array = get_tz_array ($year, $city);
			my $tz_str = serialize_tz_array($tz_array);
			print (CITY_INF $city . $tz_str);
		}
		
		close (CITY_INF);
		if($error){
			unlink "$path$city_inf.txt";
			die "Please correct $error errors.\n";
		}
		else{
			print "\nReady. Check coords in $path$city_inf.txt.\nMay I continue calculations (y/n)? ";
		}
		my $ans=<STDIN>;
#		print ">$ans<";
		chomp($ans);
		die "Calculation cancelled.\n" unless $ans eq 'y';
	}
	#####################################
		open(InF, "<$path$city_inf.txt") or die "No file";
		$/="\n";
		my @clist=<InF>;
		die "Input error=".scalar(@clist)." in $path$city_inf.txt" if scalar(@clist)<2;
#		print join("**", @clist);
		close(InF);
	#	die "@cities";
		my $i=0;
		our $city;
	#	undef $/ ;
		my $newdir=ensure_slash(sprintf('%sdata/archive/%d',$mypath,$year));
		mkdir $newdir;
		$newdir=ensure_slash("$newdir/$city_inf");
		my $arcdir=$mypath.'data';
		if(!-d $newdir){
			mkdir $newdir or die "$newdir: $!";
		}
		my $geomask=sprintf('%sdata/archive/%d/geo0-*.bin',$mypath, $year);
		foreach my $cit(@clist){
			$outbuf='';
			chomp($cit);
#print "\n>>>> $cit <<<<<\n";
			$cit=~s/\A\s*\"(.+)\"\s*\Z/$1/is;
			next if $cit=~/\A\s*\Z/is;
			next if $cit=~/\#/is;
			next if $cit!~/\d/is;
			my @params=split(/\|/is, $cit);
			$fname=$newdir.sprintf('/Data%04d.dat',$i);
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
				warn join('`', @params);
				$city_info=pack('Nnnn' 
					, $params[6] #id
					, $params[1] * 100 #lon
					, $params[2] * 100 #lat
					, $params[5] #alt
				);
				my $header=pack('nCCnna*a*a*',
					$year, $month, $day, length($city_info), $day_count, $outbuf, $dstbuf, $city_info);
	{
		use bytes; warn length($header) .'=='.unpack('H*', $header);
					warn length($city_info) .'='.unpack('H*', $city_info);
	}					
				if(!$tzonly){
					my $alt=$params[5];
					my @bins=glob($geomask);
					foreach (@bins){
						unlink($_);
					}
					my $invoke=ensure_slash($mypath."mutter2/mutter2 $year geo0- $params[1] $params[2] $alt");# electio";
#$invoke=ensure_slash("echo 1");# electio";
					print "$invoke\n";
					my $res=system($invoke);
					die "Cancelled, result=$res" if $res;
					
					open(OutF, ">$fname") or die "$! $fname";
					binmode(OutF);
					print OutF $header;
					close(OutF);

					@bins=glob($geomask);
					die "No files to pack: $geomask" if $#bins<0;
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
		unlink "$arcdir/$year/$city_inf.zip";
		open(InF, ">$newdir/$city_inf.txt");
		print(InF join("\n", @clist));
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
		print "\nHeaders replaced: $hrepl.\n" if $tzonly;
}

sub calc_dst{
	my $buf;
 	return $buf;
}

sub ctime2number{
	$_[0]=~/\w{3} (\w{3}) (\d+) (\d\d):(\d\d):(\d\d) (\d{4})/is;
	return sprintf("%04d%02d%02d%02d%02d%02d", $6, $mon{$1}+1, $2, $3, $4, $5);
}

sub tz_check{
	my($fname, $header, $comment)=@_;
	open(InF1, "<$fname") or die "$! $fname";
	binmode(InF1);
	my @data=<InF1>;
	close(InF1);
	my $body=join('', @data);
	$body =~/^.{4}(..)..(..)/s;
	my ($cust_data_len, $city_name_len, $tz_offset) = 
		(unpack('n', $1), unpack('n', $2), unpack('n', $3));
	my $hlen = 10 + $cust_data_len + $city_name_len;
	$hlen += 8 unless ($tz_offset & 0x8000) > 0;
	warn "hlen = $hlen, header len =".length($header)."\n";
	$body=~/^(.{$hlen})/s;
	my $oldhdr=$1;
	if($oldhdr ne $header){
		warn("*** $comment: Replaced header in $fname");
		print "old: ".unpack("H*",$oldhdr)."\nnew: ".unpack("H*",$header)."\n";
		$body=~s/^.{$hlen}/$header/s;
#		return 1;
		open(OutF, ">$fname") or die "$! $fname";
		binmode(OutF);
		print OutF $body;
		close(OutF);
		return 1;
	}
	return 0;
}

sub jul{ # y, m, d, hr
	my $invoke=ensure_slash($mypath."mutter2/mutter2 ".
			sprintf("%04d jul %02d %02d %f", 
			$_[0], $_[1], $_[2], $_[3]));# 
	$invoke=`$invoke`;
	chomp($invoke);
	return $invoke;
}

sub revjul{
	my $invoke=ensure_slash($mypath."mutter2/mutter2 2000 revjul $_[0]");
	$invoke=`$invoke`;
	chomp($invoke);
	return $invoke;
}

sub dow{ # julday, out: 0-Mon, 1-Tue, ..., 6-Sun 
	my $invoke=ensure_slash($mypath."mutter2/mutter2 2000 dow $_[0]");
	$invoke=`$invoke`;
	chomp($invoke);
	return $invoke;
}
=head
sub decode_time{
	my $tm=undef;
	my $wday;
	my ($year, $str, $dst)=@_;
	print "$str: ";
	my $hr=0;
	$hr=$1 if $str=~s/\@([\d\+\-\.]+)/\@/is;
	if($str=~/(\d+)(\w{3})\@/is){ # 01Apr@3
		$tm=jul($year, $mon{$2}+1, $1, 0);
	}
	elsif($str=~/last/is){ # LastSunMar@2
		my @pp=$str=~/last(\w{3})(\w{3})\@/is;
		my $month=$mon{$pp[1]}+1; # next month
		$tm=jul($year, $month+1, 0, 0);
		$wday=dow($tm);
		$pp[0]=$wd{$pp[0]}+6;
		$wday=($wday+7-$pp[0])%7;
		$tm=jul($year, $month+1, -$wday, 0);
	}
	if($str=~s/first(\w{3})//is){ 
		my($after,$week_day,$m)=(1,$wd{$1}+6);
#		die "($after, $week_day, $m)";
		if($str=~/after(\d+)(\w{3})\@/is){ # FirstSunAfter18Mar@2
			($after,$m)=($1+1,$mon{$2},$3);
		}
		else{
			$str=~/(\w{3})\@/is; # FirstSunMar@2
			$m=$mon{$1};
		}
#		die "($year,$m,$after, $hr)";
		$tm=jul($year, $m+1, $after, 0);
		$wday=dow($tm);
		$wday=(7+$week_day-$wday)%7;
		$tm=jul($year, $m+1, $after+$wday, 0);
		
	}
#print "$tm";
	my $tmstr=revjul($tm);
#	my $tmstr=POSIX::ctime($tm);
#	print "\t$tmstr, $tm";
	$tm=$tm-$epoch;
	$tm=$tm*24+$hr;
	$tm*=60;
#	$tm=($tm+$tz_ofs)/60;
	if(wantarray){
		return ($tm);#, $tmstr);
	}
	else{
		return $tm;
	}
}

sub decode_time2{
	my $tm=undef;
	my ($year, $str, $dst)=@_;
	print "$str: ";
    # extracting hours after @ into $hr, $mn, $sc
	$str=~s/\@([\d\+\-\.]+)/\@/is;
	my $hr_frac=$1*3600;
	my $hr=int($hr_frac/3600);
	$hr_frac-=$hr*3600;
	my $mn=int($hr_frac/60);
	$hr_frac-=$mn*60;
	my $sc=$hr_frac;
	if($str=~/(\d+)(\w{3})\@/is){ # 01Apr@3
		$tm=POSIX::mktime($sc, $mn, $hr, $1, $mon{$2},$year-1900,0,0,-1);
	}
	elsif($str=~/last/is){ # LastSunMar@2
		my @pp=$str=~/last(\w{3})(\w{3})\@/is;
		my $month=$mon{$pp[1]}+1; # next month
		$tm=POSIX::mktime($sc, $mn, $hr, 0, $month,$year-1900,0,0,-1);
		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime($tm);
		$pp[0]=$wd{$pp[0]};
		$wday=($wday+7-$pp[0])%7;
		$tm=POSIX::mktime($sec,$min,$hour,$mday-$wday,$m,$y,0,0,-1);
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
		my ($sec,$min,$hour,$mday,$y,$wday,$yday);
		($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime($tm);
		$wday=(7+$week_day-$wday)%7;
		$tm=POSIX::mktime($sec,$min,$hour,$mday+$wday,$m,$y,0,0,-1);
		
	}
	my $tmstr=POSIX::ctime($tm);
#	print "\t$tmstr";
	$tm=($tm+$tz_ofs)/60;
	if(wantarray){
		return ($tm, $tmstr);
	}
	else{
		return $tm;
	}
}
=cut
sub writeUTF
{
	my $param=shift;
	my $len=0;
	{
		use bytes; $len=length($param);
	}
	$outbuf.=pack('na*', $len, $param);
}

sub data_check
{
	#in: fname, year, cityname
	my ($data_year, $dc_len, $data_city, $body);
#	print " data_check(".join(',', @_).")\n";
	open(FILE, "<$_[0]");
	binmode(FILE);
	read(FILE, $data_year,4);
	$data_year=unpack('n',$data_year);
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

sub ensure_slash{
	$_[0]=~s/\//\\/isg if $winda;
	return $_[0];
}

sub get_tz_array { # $year, $timezone
	my ($year, $tz_name) = @_;
	my $tz_array = get_zoneinfo($tz_name);
	print Dumper($tz_array);
	return $tz_array;
}

sub get_zoneinfo { # $tz_name
	my $tz_name = shift();
	if (!exists ($zoneinfo{$tz_name})) {
		$zoneinfo{$tz_name} = parse_tz ("$TIMEZONE_DIR/$tz_name");
	}
	return $zoneinfo{$tz_name};
}

sub serialize_tz_array { # $tz_array
	my $tz_str = '?';
	return $tz_str;
}

sub parse_tz { # $tz_filename
	my $input_file = shift();
    my ($leapcnt, $timecnt, $typecnt, $charcnt) = (0, 0, 0, 0);
    my $INF;
	open ($INF, "<$input_file") or die "$!: $input_file";
	binmode($INF);
    # read header
#    die readInt ($INF);
    seek ($INF, 28, 0);
    $leapcnt = readInt ($INF);
    $timecnt = readInt ($INF);
    $typecnt = readInt ($INF);
    $charcnt = readInt ($INF);
#    die "$leapcnt, $timecnt, $typecnt, $charcnt";
      # load DST transition data
    my @transTimes;
    for (my $i = 0; $i < $timecnt; ++$i) {
		$transTimes[$i] = readInt ($INF);
	}
	my @transTypes;
    for (my $i = 0; $i < $timecnt; ++$i) {
		$transTypes[$i] = readByte ($INF);
	}
      # load TZ type data
    my @offset;
    my @dst;
    my @idx;
    for (my $i = 0; $i < $typecnt; ++$i) {
		$offset[$i] = readInt ($INF);
		$dst[$i] = readByte ($INF);
		$idx[$i] = readByte ($INF);
    }
	my @str;
    for (my $i = 0; $i < $timecnt; ++$i) {
		$str[$i] = readByte ($INF);
	}
	close ($INF);
	  # convert type data
	my @tz;
	for (my $i = 0; $i < $typecnt; ++$i) {
		# find string
		my $pos = $idx[$i];
		my $end = $pos;
		my $name = '';
		while ($str[$end] != 0) {
			$name .= pack ('c', $str[$end]);
			++$end;
		}
		
		$tz[$i] = {
			name => $name,
			gmt_ofs_sec => $offset[$i],
			is_dst => int($dst[$i] != 0),
		};
	}
	return {
		leapcnt => $leapcnt,
		timecnt => $timecnt,
		typecnt => $typecnt,
		charcnt => $charcnt,
		transTimes => \@transTimes,
		transTypes => \@transTypes,
		tz => \@tz
	};
}

sub readInt { # handle
	my $integer;
	my $bytes_read = read ($_[0], $integer, 4);
   	if ($bytes_read != 4) {
		die "Read $bytes_read: $!";
	}
	$integer = unpack ('N', $integer);
	if ($integer >= pow(2, 31)) {
		$integer -= pow (2, 32);
	}
	return $integer;
}

sub readByte { # handle
	my $byte;
	my $bytes_read = read ($_[0], $byte, 1);
   	if ($bytes_read != 1) {
		die "Read $bytes_read: $!";
	}
	$byte = unpack ('c', $byte);
	return $byte;
}

sub dump_zone { # $tz_name
	my $tz_name = shift();
	my $tz = get_zoneinfo ($tz_name);
	#tz_dump($tz, -pow (2, 31));
	tz_dumpdst ($tz);
	#tz_dumpleap ($tz);
	#tz_dump ($tz, pow (2, 31) - 1);
}

# Dump daylight savings time transitions
sub tz_dumpdst { # $tz
	my $tz = shift();
	my $transTimes = $tz->{transTimes};
	my $transTypes = $tz->{transTypes};
	my $i = 0;
	foreach my $transTime (@$transTimes) {
		#tz_dump($tz, $t - 1);
		my $tzType = $tz->{tz}->[$transTypes->[$i]];
		tz_dump($tz, $transTime, $tzType);
		++$i;
	}
}

sub tz_dump { # $tz, $time, $tz_type
	my ($tz, $time, $tz_type) = @_;
	print $time . " > " . gmtime($time) . " =" . 
		' name: ' . $tz_type->{name} . 
		' gmt_ofs_sec: ' . $tz_type->{gmt_ofs_sec} . 
		' is_dst: ' . $tz_type->{is_dst} . "\n";
}
