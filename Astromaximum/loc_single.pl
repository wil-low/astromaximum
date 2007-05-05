use strict;
use POSIX;
#use warnings;
use lib 'D:/Willow/prj/astrology/nomad_prj/'; 
use lib 'd:/projects/nomad_prj';
use tools;
	my $i=$#ARGV+1;
	tools::join_datafiles($i, "Astromaximum/src/location.dat", \@ARGV);
	print "Astromaximum/src/location.dat written";