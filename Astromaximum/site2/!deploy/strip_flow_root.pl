#!/usr/bin/perl
use strict;
use warnings;

my @data = <>;
my $body = join ('', @data);
$body =~ s/<flowRoot.+?<\/flowRoot>//s;
print $body;
