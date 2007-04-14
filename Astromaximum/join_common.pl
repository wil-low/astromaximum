use strict;
use warnings;
use POSIX;
use lib 'D:/Willow/prj/astrology/nomad_prj/';
use lib 'd:/projects/nomad_prj';
use tools;

our $imei=$ARGV[1];

if(!$ARGV[0] or $imei!~/\d{15}/is){
	die "Usage: <dest_dir> <IMEI>\n";
}
$imei='359593001109710' unless $imei;

#die sprintf('%x',substr($imei,0,8));

$0=~/(.+\\)/is;
my $mypath=$1;

my ($year, $day_count)=tools::get_year($mypath);
my ($month, $day, $hour, $min)=(1,1,0,0);

our $outp=$ARGV[0];
print "imei=$imei\n";
our $path=$mypath.'mutter\\output\\';

my $header=pack('nCCCCn',$year, $month, $day, $hour, $min, $day_count);

my $InF=undef;
my $OutF;
undef $/ ;
	open($OutF, ">$outp\\common.dat") or die "No file $outp\\".'common.dat';
	binmode($OutF);
	print $OutF $header;
	close($OutF);

print $path."*.bin\n";
my @bins=glob("$path".'*.bin');

my $counter=0;
foreach my $ff(@bins){
	if($ff=~/(rise|set|navroz|geo|nakshatra)/is){
		next;
	}
	tools::writeData($ff, "$outp\\common.dat", substr($imei,$counter++,1));
	if($counter>=length($imei)){
		$counter=0;
	}
}	

