package amtools;

use strict;
use CGI ':standard';
use CGI::Carp 'fatalsToBrowser';

sub join_datafiles2 # destfile, data_listref
{
	open(OUTF, ">$_[0]") or die "No file";
	my @bins=@{$_[1]};
	my @buf;
	binmode(OUTF);
	print OUTF pack('n',$#bins+1);
	my $i=0;
	foreach (@bins){
		print OUTF pack('n',length($_));
	}
	foreach (@bins){
		print OUTF $_;
	}
	close(OUTF);
}

sub random # path, extension if file, undef if dir
{
	my ($fn,$id,$flag);
	do{
		$id=''; $flag=1;
		for(my $i=0; $i<12; $i++){
			$id.=int(rand(10));
		}
		$fn="$_[0]/$id$_[1]";
		if($_[1]){
			$flag=0 if -f $fn;
		}
		else{
			$flag=0 if -d $fn;
		}
	}while(!$flag);
	return ($fn, $id);
}

1;