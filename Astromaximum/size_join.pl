#!/usr/bin/perl
use strict;
use warnings;
use POSIX;

$0=~/(.+\\)/is;
our $path=$1;
my($InF,$OutF);

join_phases(9);
join_phases(12);

sub join_phases
{
	my $size=shift;
	my @buf;
	my @bodies;
	my @bins=qw(service zodiac planet opaqplanet aspect);
	foreach my $it(@bins){
		$it=$path.'images\\size_png\\'.$it.$size.'.png';
	}
	open($OutF, ">$path".'Astromaximum\\src\\res\\sz'.$size.'.dat') or die "No file";
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
