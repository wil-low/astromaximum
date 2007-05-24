#!/usr/bin/perl

use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 10;  # max 100K posts
$CGI::DISABLE_UPLOADS = 1;  # no uploads
use tools;


my $dsn = "DBI:mysql:database=amax;host=localhost";
my $dbh = DBI->connect($dsn, 'root', '');

#tools::cookie_check($dbh);

my $type=undef;
$type='r' if defined(param('r'));
my $fn="../files/".param($type).".$type";
if($type && -f $fn){
	print header(-type=>'application/octet-stream', -attachment=>"cities.ja$type");
	my $data;
	open(FFF, "<$fn");
	read(FFF, $data, -s $fn);
	close(FFF);
	print $data;
}
else{
  print header('text/html','204 No response');
}
$dbh->disconnect;
