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

$imei->pack();
$bt_imei->pack(-fill=>"x");
$frame->Label(-text=>"or")->pack(-pady=>10);
$lbox->insert('end', @list );
$lbox->selectionSet(2);
$lbox->pack();

$bt_timebomb->pack(-fill=>"x");

$frame->pack(-side=>"left");

=head
my $cities=$main->Listbox();

my @inis=glob($path."..\\mutter\\geo\\*.ini");
foreach my $ini (@inis){
	open(INI,"<$ini") or die "No file";
	my $country=<INI>;
	die $country;
	$country=~/:(.+)\n/is;
	$country=
	close(INI);
}



$cities->pack(-fill=>"both", -expand=>1);
=cut

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
