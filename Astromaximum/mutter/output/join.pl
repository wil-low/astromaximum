use strict;
use POSIX;

our $imei='359593001109710';
#our $imei='359308007701623';
#die sprintf('%x',substr($imei,0,8));

my ($year, $month, $day, $hour, $min, $day_count)=(2007,1,1,0,0,365);
$0=~/(.+\\)/is;

my $header=pack('nCCCCn',$year, $month, $day, $hour, $min, $day_count);
our $path=$1;

my $InF=undef;
my $OutF;
undef $/ ;
	open($OutF, ">$path".'common.dat') or die "No file";
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


sub writeData
{
	my $bintype=shift;
	my $src=shift;
	open($OutF, ">>$path".'common.dat') or die "No file";
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
