use strict;
use POSIX;
use Unicode::String;
use Encode;



my $country='Ukraine, Kiev';
my $tz='+2.0   LastSunMar@3              LastSunOct@4              UKR_Ukraine, Kiev';
my ($year, $month, $day, $hour, $min, $day_count)=(2007,1,1,0,0,365);

my $sqpath='d:\\projects\\astro\\';
#my $sqpath='D:\\Willow\\prj\\astrology\\';

my $tz_ofs=0;
{
#		warn $tm;
		my $tm=time;
		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = gmtime();
		my $tm2=POSIX::mktime($sec, $min, $hour, $mday, $m,$y,0,0,-1);
		
		$tz_ofs=$tm-$tm2;
}
my %mon=qw(Jan 0 Feb 1 Mar 2 Apr 3 May 4 Jun 5 Jul 6 Aug 7 Sep 8 Oct 9 Nov 10 Dec 11);
my %wd=qw(Sun 0 Mon 1 Tue 2 Wed 3 Thu 4 Fri 5 Sat 6);

my $dstbuf=calc_dst($tz);

$0=~/(.+\\)/is;
our $path=$1;
our $outbuf;
our $fname;
my $InF=undef;
my $OutF;
my @cities;

	
	
	open($InF, "<$path".'geo\\'."$country\.txt") or die "No file";
	@cities=<$InF>;
	close($InF);
	my $i=0;
	our $city;
	undef $/ ;
	foreach my $cit(@cities){
		$outbuf='';
		chomp($cit);
		next if $cit=~/\#/is;
		next if $cit!~/\d/is;
		my @params=split(/\|/is, $cit);
		my $invoke=$path."mutter.exe 2007 geo$i- $params[1] $params[2]";
		print "$invoke\n";
		system($invoke);
		$city=$params[0];
		$fname=$path.sprintf('geo\\Data%02d.dat',$i);
		writeUTF($city);
		my $header=pack('SCCCCSa*a*',$year, $month, $day, $hour, $min, $day_count, $outbuf, $dstbuf);
		
		print "$fname\n";
		open($OutF, ">$fname") or die "No file";
		binmode($OutF);
		print $OutF $header;
		close($OutF);
		
		my @bins=glob($path."output\\geo$i-\*.bin");
		my $counter=0;
		print join(@bins,"\n");
		foreach my $ff(@bins){
				writeData(2, $ff, 0);
		}	
		$i++;
	}
	join_phases($i);



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

sub join_phases
{
	my $size=shift;
	my @buf;
	my @bodies;
	my @bins=glob($path.'geo\\Data*.dat');
	open($OutF, ">$path".'geo\\locations.dat') or die "No file";
	binmode($OutF);
	print $OutF pack('n',$#bins+1);
	my $i=0;
	foreach my $ff(@bins){
		open($InF, "<$ff") or die "No file";
		binmode($InF);
		undef $/ ;
		@buf=<$InF>;
		close($InF);
		$bodies[$i]="@buf";
		print $OutF pack('n',length($bodies[$i]));
		++$i;
	}
	foreach my $png(@bodies){
		print $OutF $png;
	}
	close($OutF);
}



sub writeData
{
	my $bintype=shift;
	my $src=shift;
	open($OutF, ">>$fname") or die "No file";
	binmode($OutF);
	open($InF, "<$src") or die "No file";
	binmode($InF);
	undef $/ ;
	my $body=<$InF>;
	close($InF);
	my $imeichar=shift;
	if(length($body)>8){
		print $OutF pack('c',$imeichar).$body; #
		print "$src\t$bintype\t$imeichar\n";
	}
	close($OutF);
}

sub writeUTF
{
	my $param=shift;
	print "$param\n";
	$param = decode("cp1251", $param);
	my $len=Unicode::String->new($param);
	$outbuf.=pack('na*', length($len), $param);
#	$outbuf.=$param;
#	die $outbuf;
}
