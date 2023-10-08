#!/usr/bin/perl
use strict;
use warnings;

my %cities;

open (WORLDS, "<worlds.csv") or die "$!: worlds.csv";
while (my $line = <WORLDS>) {
	$line =~ s/[\n\r]//g;
	my @fields = split (/;/, $line);
	if (scalar(@fields) < 9) {
		next;
	}
	if (defined($cities{$fields[0]})) {
		warn "Duplicate city: $line; was $cities{$fields[0]}";
	}
	else {
		$cities{$fields[0]} = "$fields[8]:$fields[7] $fields[0]";
	}
}
close (WORLDS);

open (INF, "<$ARGV[0]") or die "$!: $ARGV[0]";
while (my $line = <INF>) {
	$line =~ s/[\n\r]//g;
	$line =~ s/^\s+//;
	$line =~ s/\s+$//;
	next if $line eq '';
	if (defined($cities{$line})) {
		print "$cities{$line}\n";
	}
	else {
		die "City not found: $line";
	}
}
close (INF);
