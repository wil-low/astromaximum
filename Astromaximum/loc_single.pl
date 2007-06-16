#!/usr/bin/perl
use strict;
use POSIX;
#use warnings;
use lib 'D:/Willow/prj/astrology/nomad_prj/'; 
use lib 'd:/projects/nomad_prj';
use tools;
	my $OUT='Astromaximum/src/locations.dat';
	my $i=$#ARGV+1;
	tools::join_datafiles($i, $OUT, \@ARGV);
	print "$OUT written";