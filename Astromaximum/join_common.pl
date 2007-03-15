use strict;
use POSIX;
use tools;

our $imei=$ARGV[1];

if(!$ARGV[0] or $imei!~/\d{15}/is){
	die "Usage: <dest_dir> <IMEI>\n";
}
$imei='359593001109710' unless $imei;

#die sprintf('%x',substr($imei,0,8));

my ($year, $month, $day, $hour, $min, $day_count)=(2007,1,1,0,0,365);
$0=~/(.+\\)/is;
our $outp=$1.$ARGV[0];
print "outp=$outp";
our $path=$1.'mutter\\output\\';

my $header=pack('nCCCCn',$year, $month, $day, $hour, $min, $day_count);

my $InF=undef;
my $OutF;
undef $/ ;
	open($OutF, ">$outp\\common.dat") or die "No file";
	binmode($OutF);
	print $OutF $header;
	close($OutF);

my @bins=glob("$path".'*.bin');
#my @bins=glob("$path".'retro09.bin');
my $counter=0;
foreach my $ff(@bins){
	if($ff=~/(rise|set|navroz|geo|nakshatra)/is){
		next;
	}
#	die pack('c',substr($imei,$counter++,1));
	tools::writeData($ff, "$outp\\common.dat", substr($imei,$counter++,1));
	if($counter>=length($imei)){
		$counter=0;
	}
	next;
}	

