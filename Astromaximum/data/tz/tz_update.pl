#!/usr/bin/perl
use strict;
use POSIX;
#use warnings;
use Encode;
$0=~/(.+[\/\\])/is;
our $mypath=$1;
$mypath='./' unless $mypath;
my $updpath=$mypath."update";
open(FIN, "<$updpath/city.txt") or die "$! in $updpath";
my @data=<FIN>;
close(FIN);
my $body=join('', @data);
Encode::from_to($body, "iso-8859-1", "utf-8");
open(FIN, ">$updpath/city-new.txt") or die "$!";
print(FIN $body);
close(FIN);
