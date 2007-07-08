#!/usr/bin/perl

use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 10;  # max 100K posts
$CGI::DISABLE_UPLOADS = 1;  # no uploads
use tools;

my $dbh = tools::db_connect();

tools::cookie_check($dbh);

print header, start_html(-title=>'Astromaximum - Session control');
print tools::adm_panel();

my $stcou = $dbh->prepare("SELECT files.id, customers.name, end_tm FROM files,customers WHERE user_id=customers.id GROUP BY files.id ORDER BY end_tm DESC");
$stcou->execute;
print "<table border=1>";
my $i=1;
while(my @row = $stcou->fetchrow_array){
	print "<tr><td>$i</td>";
	foreach (@row){
		print "<td>$_</td>";
	}
	print "</tr>\n";
	$i++;
}
print "</table>";
$stcou->finish;

$dbh->disconnect;

print end_html;
