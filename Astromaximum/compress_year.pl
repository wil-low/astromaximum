#!/usr/bin/perl
use strict;
use warnings;
require 'genconst.pm';

exec ("bash compress_year.sh $ARGV[0] " . $const::CALCULATIONS_DIR);

