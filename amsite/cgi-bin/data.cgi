#!/usr/bin/perl

use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 10;  # max 10K posts
$CGI::DISABLE_UPLOADS = 1;  # no uploads
use tools;


my $dbh = tools::db_connect();

#tools::cookie_check($dbh);

my $type=undef;
$type='r' if defined(param('r'));
$type='d' if defined(param('d'));
$type='t' if defined(param('t'));
my $dig=param($type);
$dig=~/(\d{4})$/is;
my $idd=$1;
my $fn="../files/".param($type).".$type";
#die $fn;
if($type && -f $fn){
	my $sth = $dbh->prepare("UPDATE files SET used=\'t\' WHERE id=$dig AND type=\"$type\"");
	$sth->execute;
	$sth->finish;
	$type='d' if $type eq 't';
	my $data;
	open(FFF, "<$fn");
	binmode(FFF);
	read(FFF, $data, -s $fn);
	close(FFF);
	print header(-type=>'application/octet-stream', -attachment=>"cities-$idd.ja$type");
	print $data;
}
else{
  print header()."No file $fn";
}
$dbh->disconnect;
