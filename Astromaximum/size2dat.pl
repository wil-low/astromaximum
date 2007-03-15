use strict;
$0=~/(.+\\)/is;

my $path=$1;

my $EV_LAST=27;

my @bins=glob("$path".'arrays\\*.txt');
foreach my $ff(@bins){
	my $InF=undef;
	our $OutF=undef;
	open($InF, "<$ff") or die "No file";
	$ff=~/.+[\\\/](\w+)\./is;
	my $fname=$1;
	open($OutF, ">$path".'Astromaximum\\src\\res\\'.$fname.'.dat') or die "No file";
	binmode($OutF);
	my $count=-1;
	my @buf=<$InF>;
	close($InF);
	my $line;
	while($line=shift(@buf)){
		$line=~s/\#.+//is;
		if($line=~/\{(.+)\}/is){
			$line=$1;
		}
		else{
			next;
		}
		my @num=split(/\,/, $line);
#		warn ">$line<" if $#num!=6;
		foreach my $nn(@num){
			print ($OutF pack('n',$nn));
			$count=$nn if $nn>$count;
		}
	#	print $line;
	}
	++$count;
	if($fname=~/tabStop/is){
		for(my $i=0; $i<24; $i++){
			print ($OutF pack('c',$count+$i));
		}
	}
	close($OutF);
}