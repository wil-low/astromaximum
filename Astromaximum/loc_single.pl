#!/usr/bin/perl
use strict;
use POSIX;
#use warnings;
use lib 'D:/Willow/prj/astrology/nomad_prj/'; 
use lib 'd:/projects/nomad_prj';
use tools;
	my $OUT;
	my @args=@ARGV;
	if(!$args[0]){
	    die "Usage loc_single.pl [l] <file list>\n";
	}
	if($args[0] eq 'l'){
	    $OUT='Astromaximum/src/l.dat';
	    shift @args;
	}
	else{
	    $OUT='Astromaximum/src/locations.dat';
	}
	my $i=$#args+1;
	tools::join_datafiles($i, $OUT, \@args);
	print "$OUT written\n";