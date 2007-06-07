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
		my $sth = $dbh->prepare("SELECT user_id from sessions where name=\"$session\"");
		$sth->execute;
		my @row = $sth->fetchrow_array;
		$sth->finish;
		if($ra>0 && $#row>=0){
			return $row[0];
		}
	}
	print redirect('http://astromaximum/cgi-bin/start.cgi');
	$dbh->disconnect;
	exit(0);
}

sub adm_panel{
	return <<ADM;
	
<p><a href='geo.cgi'>Geo</a>
&nbsp;&nbsp;<b>Admin:&nbsp;
<a href='http://localhost/Tools/phpMyAdmin/'>phpMyAdmin</a> 
<a href='http://localhost/Docs/MySQL4/index.html'>mySQL docs</a> 
<a href='sessions.cgi'>Sessions</a> 
<a href='files.cgi'>Files</a> 
<a href='upload.cgi'>Upload</a>
</b></p>
ADM

}

1;