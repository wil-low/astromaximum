#!/usr/bin/perl
use strict;
use warnings;

die "Converts year and day count in Data*.dat to Big-Endian.\n" if ! $ARGV[0];

open (INF, "<$ARGV[0]") or die "$!: $ARGV[0]";
binmode(INF);
$/='';
my $data = <INF>;
close(INF);
$data =~ s/^(.)(.)/$2$1/;
$data =~ s/^(.{6})(.)(.)/$1$3$2/;
print "$ARGV[0]\n";
open (OUTF, ">$ARGV[0]") or die "$!: $ARGV[0]";
binmode(OUTF);
print (OUTF $data);
close (OUTF);
