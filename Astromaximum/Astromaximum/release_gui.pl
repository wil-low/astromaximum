use strict;
use Tk;
$0=~/(.+\\)/is;
our $path=$1;
my $main = new MainWindow(-title=>"Astromaximum");
my $frame=$main->Frame();
my $imei=$frame->Entry();
my $bt_imei=$frame->Button(-text=>"IMEI", -command=>\&do_imei);
my $lbox = $frame->Listbox(-height=>0,-activestyle=>"dotbox");
my @list = ( "5", "10", "20", "30", "60" );
my $bt_timebomb=$frame->Button(-text=>"Time Bomb", -command=>\&do_timebomb);
my $bt_geo=$main->Button(-text=>"Generate Geo", -command=>\&do_geo);

$imei->pack();
$bt_imei->pack(-fill=>"x");
$frame->Label(-text=>"or")->pack(-pady=>10);
$lbox->insert('end', @list );
$lbox->selectionSet(2);
$lbox->pack();

$bt_timebomb->pack(-fill=>"x");

$frame->pack(-side=>"left");

#=head
my $lbcities=$main->Scrolled('Listbox',-scrollbars=>'e');
$lbcities->configure(-selectmode=>'multiple');

my %sets;
my @inis=glob($path."..\\GeoAM\\geo\\*.ini");
foreach my $ini (@inis){
	$ini=~/.+\\(.+?)\.ini/is;
	$ini=$1;
	open(INI,"<$path"."..\\GeoAM\\geo\\$ini\\$ini.txt") or die "No file"."..\\GeoAM\\geo\\$ini\\$ini.txt";
	my @cset=<INI>;
	close(INI);
	$sets{$ini}=\@cset;
	foreach (@cset){
		my $ci=$_;
		next if $ci=~/\#/is;
		$ci=~s/\|(.+)\|/, /is;
		$lbcities->insert('end',$ci);
	}
}

$lbcities->pack(-fill=>"both", -expand=>1);
$bt_geo->pack(-fill=>"x");
#=cut

MainLoop();

sub do_timebomb {
	print "timebomb\n";
	my $timeout=$lbox->get("active");
	system("release.bat tb $timeout");
}

sub do_imei {
	print "imei\n";
	my $code=$imei->get();
	system("release.bat imei $code");
}

sub do_geo {
	print "geo\n";
}
