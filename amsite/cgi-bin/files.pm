package files;
use strict;
use warnings;
use CGI ':standard';
use DBI;
use CGI::Carp 'fatalsToBrowser';
$CGI::POST_MAX=1024 * 10;  # max 100K posts
$CGI::DISABLE_UPLOADS = 1;  # no uploads
use tools;

sub get_content{ # dbh, userid, hashref
	my $dbh = $_[0];
	my $out=tools::adm_panel();
	
	my $stcou = $dbh->prepare("SELECT files.id, customers.name, end_tm FROM files,customers WHERE user_id=customers.id GROUP BY files.id ORDER BY end_tm DESC");
	$stcou->execute;
	$out.="<table border=1>";
	my $i=1;
	while(my @row = $stcou->fetchrow_array){
		$out.="<tr><td>$i</td>";
		foreach (@row){
			$out.="<td>$_</td>";
		}
		$out.="</tr>\n";
		$i++;
	}
	$out.="</table>";
	$stcou->finish;
	return $out;
}

1;