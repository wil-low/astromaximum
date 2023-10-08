#!/usr/bin/perl
use strict;
use warnings;
use File::Find;

open (OUTF, ">worlds.csv") or die "$!: world.csv";

print (OUTF "city;state;country;latitude;longitude;altitude;timezone;hex;filename\n");
find ({wanted => \&location_found, no_chdir => 1}, '../world');
close (OUTF);

sub location_found {
	if ($File::Find::name =~ /(\w+)\.world$/) {
		my $fn = $1;
		open (INF, "<$File::Find::name") or die "$!: $File::Find::name";
		while (my $line = <INF>) {
			$line =~ s/[\n\r]//g;
			my @fields = split (/;/, $line);
			if (scalar(@fields) < 8) {
				next;
			}
			print (OUTF "$line$fn\n");
		}
	}
}
