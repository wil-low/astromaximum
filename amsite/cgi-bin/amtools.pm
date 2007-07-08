package amtools;

use strict;
use CGI ':standard';
use CGI::Carp 'fatalsToBrowser';

our $unzip=q(unzip -q %s -d %s );
#our $unzip=q("d:/Program Files/WinRAR/WinRar.exe" x %s * %s\ );
our $zip=q(wd=`pwd`; cd %s; zip -qrvm $wd/%s.r * > null;cd $wd);
#our $zip=q(zip -r %s.r %s/*);
#our $zip=q("d:/Program Files/WinRAR/WinRar.exe" a -afzip -r -ep1 %s.r %s/*);

sub join_datafiles2 # destfile, data_listref
{
	open(OUTF, ">$_[0]");
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

sub rm_all{ #dir to erase
	my $dir=shift;
	unlink glob("$dir/*.*");
#	foreach (glob("$dir/*.*")){
#		if(-f $_){
#			unlink $_;
#		}
#		else{
#			rm_all($_);
#		}
#	}
	rmdir($dir);
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
