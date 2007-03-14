use strict;
use POSIX;
my @file=("Interpreter","CustomTime");
my @sign=(pack('N',0x01234567),pack('N',0x89abcdef));


$0=~/(.+\\)/is;
die "Usage: <classes dir> <timeout>\n" if($#ARGV!=1);
my $class_dir=$ARGV[0];
my $mins=$ARGV[1];
#$class_dir=$1.'build\\midp2y2007notest\\compiled';
#my $tz_ofs=0;
#		my $tm=time;
		my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime();
		my $tm2=POSIX::mktime($sec, $min, $hour, $mday, $m,$y,0,0,-1)*1000;
#		$tz_ofs=$tm-$tm2;
		
print "Installing time bomb +$mins minutes...\n\n";
#print POSIX::strftime( "Current time is %B %d, %Y - %H:%M:%S GMT\n", $sec,$min,$hour,$mday,$m,$y,$wday );
print "Begin time:  ", timebomb_install($tm2,0);
$min+=$mins;
#print POSIX::strftime( "Deadline time is  %B %d, %Y - %H:%M:%S GMT\n", $sec,$min,$hour,$mday,$m,$y,$wday );
$tm2=POSIX::mktime($sec, $min, $hour, $mday, $m,$y,0,0,-1)*1000;
print "  End time:  ", timebomb_install($tm2,1);

print "Finished.\n";

sub timebomb_install # time, index
{
	my $tm2=int($_[0]/4096);
	my $class="$class_dir\\".$file[$_[1]].'.class';
	undef $/ ;
	open(InF, "<$class") or die "No file $class";
	binmode(InF);
	my $body=<InF>;
	close(InF);
	my $hextm=pack("N",$tm2);
#	print $tm2,',',unpack("H*",$hextm);
	my $pos=index($body, $sign[$_[1]]);
	if($pos>=0){
		substr($body, $pos, length($hextm))=$hextm;
		open(InF, ">$class") or die "No file $class";
		binmode(InF);
		print InF $body;
		close(InF);
	  my($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime($tm2*4.096);
		return POSIX::strftime( "%B %d, %Y - %H:%M:%S ", $sec,$min,$hour,$mday,$m,$y,$wday).' 0x'.unpack("H*",$hextm)."\n";
	}
	else{
		return "Operation failed on $class!!!\n";
	}
}
