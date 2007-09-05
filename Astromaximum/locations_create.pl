#!/usr/bin/perl
use strict;
use POSIX;
#use warnings;
$0=~/(.+[\\\/])/is;
my $path=$1;
require $path.'tools.pm'; 
	my $OUT;
	if(!$ARGV[0]){
	    die "Usage: locations_create.pl <year> <city list> <dest file>\n";
	}
	my @fn;
	open(IN, "<$ARGV[1]") or die "error $!: $ARGV[1]\n";
	while(my $ln=<IN>){
		if($ln=~/(\w+):(Data\d\d)/is){
			push(@fn, $path."mutter/output/archive/$ARGV[0]/$1/$2.dat");
		}
	}
	close(IN);
	my $i=scalar(@fn);
	tools::join_datafiles($ARGV[0], $i, $ARGV[2], \@fn);
	print "$ARGV[2] written\n";