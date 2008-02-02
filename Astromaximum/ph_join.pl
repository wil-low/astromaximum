#!/usr/bin/perl
use strict;
#use warnings;

#for(my $i=0; $i<=$#str; $i++){
#  $str[$i]=~/\"(.+?)\"/is;
#  my $ss=$1;
#  my $strr=$ss;
#  $ss=str_encode($strr, $i);
#  print("[".$strr."]%".$i.",3% ".$ss."\n");
#}
#
#die ;
$0=~/(.+\/)/is;
our $path=$1;
my($InF,$OutF);

join_phases(28);
join_phases(50);

sub join_phases
{
	my $size=shift;
	my @buf;
	my @bodies;
	my @bins=glob($path.'images/phasesgif/ph'.$size.'-*.png');
	open($OutF, ">$path".'Astromaximum/src/res/ph'.$size.'.dat') or die "No file";
	binmode($OutF);
	print $OutF pack('n',$#bins+1);
	my $i=0;
	foreach my $ff(@bins){
		open($InF, "<$ff") or die "No file";
		binmode($InF);
		@buf=<$InF>;
		close($InF);
		$bodies[$i]=join('',@buf);
		print $OutF pack('n',length($bodies[$i]));
		++$i;
	}
	foreach my $png(@bodies){
		print $OutF $png;
	}
	close($OutF);
}

