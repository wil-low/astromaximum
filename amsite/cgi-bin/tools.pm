package tools;

use strict;
use CGI ':standard';

sub cookie_check{
	my $session=cookie('session');
	my $dbh=shift;
	if($session=~/^[0-9a-f]+$/is){
		my $stat="UPDATE sessions SET tm_end=NOW() WHERE name=\"$session\"";
		my $sth = $dbh->prepare($stat);
		my $ra=$sth->execute;
		$sth->finish;
		return if $ra>0;
	}
	print redirect('http://astromaximum/cgi-bin/start.cgi');
	$dbh->disconnect;
	exit(0);
}


1;