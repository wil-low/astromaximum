#!/usr/bin/perl
use strict;
use POSIX;

our $imei='000000000000000';
#our $imei='359308007701623';
#die sprintf('%x',substr($imei,0,8));
our $dest='.';
if(scalar(@ARGV)==0){
    die "Usage: <year> [dest dir] [IMEI]\n";
}
my ($year,$month, $day, $hour, $min, $day_count)=($ARGV[0],1,1,0,0,365);
if($year%100==0){
	if($year%400==0){
		$day_count++;
	}
}
else{
	if($year%4==0){
		$day_count++;
	}
}
	 	
	
$0=~/(.+\/)/is;

if($ARGV[2]){
    if($ARGV[2]=~/^\d{15}$/is){
	$imei=$ARGV[2];
    }
    else{
	print "Invalid IMEI=$ARGV[2],using $imei\n";
    }
}

$dest=$ARGV[1] if $ARGV[1];
my $header=pack('nCCCCn',$year, $month, $day, $hour, $min, $day_count);
our $path=$1;

$path.="mutter/output/archive/$year/";
my $InF=undef;
my $OutF;
undef $/ ;
	open($OutF, ">$dest/".'common.dat') or die "No file";
	binmode($OutF);
	print $OutF $header;
	close($OutF);

my @bins=glob("$path".'*.bin');
#my @bins=glob("$path".'retro09.bin');
my $counter=0;
foreach my $ff(@bins){
	if($ff=~/(rise|set|navroz|geo|nakshatra|degall|aphetics)/is){
		next;
	}
#	die pack('c',substr($imei,$counter++,1));
	writeData(1, $ff, substr($imei,$counter++,1));
	if($counter>=length($imei)){
		$counter=0;
	}
	next;
}	
print $dest."/common.dat ($year) saved.\n";

sub writeData
{
	my $bintype=shift;
	my $src=shift;
	open($OutF, ">>$dest/".'common.dat') or die "No file";
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
