package tools;

use strict;
use CGI ':standard';
use DBI;

my $db_name='usr_web42_1';
my $db_port=3306;

my $db_superuser='web42';
my $db_superuser_pwd='vSZBWppx';
my $db_user='user';
my $db_user_pwd='user';

our $dir_source='../source';
our $dir_inbox='../inbox';
our $dir_files='../files';

sub db_connect{
	my $dsn = "DBI:mysql:database=$db_name;host=localhost";
	my $dbh = DBI->connect($dsn, $db_superuser, $db_superuser_pwd);
	return $dbh;
}

sub cookie_check{
	my $dbh=shift;
	my $session=cookie('session');
	$session=shift unless $session;
	if($session=~/^[0-9a-f]+$/is){
		my $stat="UPDATE sessions SET tm_end=NOW() WHERE name=\"$session\"";
		my $sth = $dbh->prepare($stat) || die $dbh->errstr;
		my $ra=$sth->execute || die $dbh->errstr;
		my $sth = $dbh->prepare("SELECT user_id, customers.name from sessions, customers where sessions.name=\"$session\" and customers.id=user_id") || die $dbh->errstr;
		$sth->execute || die $dbh->errstr;
		my @row = $sth->fetchrow_array;
		$sth->finish;
		if($#row>=0){
			return @row;
		}
	}
#	print redirect('start.cgi');
#	$dbh->disconnect;
#	exit(0);
	return (0,undef);
}


sub adm_panel{
	return <<ADM;
	
<p><a href='start.cgi?p=geo'>Geo</a>
&nbsp;&nbsp;<b>Admin:&nbsp;
<a href='http://localhost/Tools/phpMyAdmin/'>phpMyAdmin</a> 
<a href='http://localhost/Docs/MySQL4/index.html'>mySQL docs</a> 
<a href='sessions.cgi'>Sessions</a> 
<a href='start.cgi?p=files'>Files</a> 
<a href='start.cgi?p=upload'>Upload</a>
</b></p>
ADM

}

1;
