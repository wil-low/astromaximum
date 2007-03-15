use strict;
use GD;
use GD::Image;
die "Please provide filename as parameter.\n" if $#ARGV!=0;
GD::Image->trueColor(1);
undef $/;
my $body; my @buff;
my $fn=
	$ARGV[0];
# 'src\common.dat';
open(InF, "<$fn" ) or die "No file";
binmode(InF);
@buff = <InF> ;
close(InF) ;
$body="@buff";

my $wh=int(sqrt(length($body)/3+1))+1;
my $img=GD::Image->new($wh,$wh);
my $ptr=0;
for(my $y=0; $y<$wh; $y++){
	for(my $x=0; $x<$wh; $x++){
		my $col=(byte($ptr)<<16)+(byte($ptr+1)<<8)+(byte($ptr+2));
		$img->setPixel($x,$y,$col);
		$ptr+=3;
	}
}
my $newbody = $img->png(9);
$fn=~s/\.\w+$/\.png/is;
open (DISPLAY,">$fn");
binmode DISPLAY;
print DISPLAY $newbody;
close DISPLAY;	


die $wh;
die $body;

sub byte {
	return ord(substr($body,shift));
}