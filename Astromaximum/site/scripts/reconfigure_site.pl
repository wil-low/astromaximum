#!/usr/bin/perl
use strict;
use warnings;
use POSIX;
require '../../genconst.pm';

my $fn = $ARGV[0];

die "Usage: reconfigure_site.pl <config.php>" if !defined($fn);

open (INF, "<$fn") or die "$!: $fn";

my $release_date = POSIX::strftime ('%Y-%m-%d', localtime);
while (<INF>) {
	s/<<VERSION>>/$const::VERSION/; 
	s/<<RELEASE_DATE>>/$release_date/;
	print;
}
close (INF);
