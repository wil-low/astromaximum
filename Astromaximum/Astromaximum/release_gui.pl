use strict;
use Tk;
$0=~/(.+\\)/is;
our $path=$1;
our $sortmode='city';
my $main = new MainWindow(-title=>"Astromaximum");
my $frame0=$main->Frame();
my $imei=$frame0->Entry();
my $bt_imei=$frame0->Button(-text=>"IMEI", -command=>\&do_imei);
my $lbox = $frame0->Listbox(-height=>0,-activestyle=>"dotbox");
my @list = ( "5", "10", "20", "30", "60" );
my $bt_timebomb=$frame0->Button(-text=>"Time Bomb", -command=>\&do_timebomb);

$imei->pack();
$bt_imei->pack(-fill=>"x");
$frame0->Label(-text=>"or")->pack(-pady=>10);
$lbox->insert('end', @list );
$lbox->selectionSet(2);
$lbox->focusFollowsMouse();
$lbox->pack();

$bt_timebomb->pack(-fill=>"x");

$frame0->pack(-side=>"left");

my $frame1=$main->Frame();
$frame1->pack(-side=>"left", -fill=>"both", -expand=>1,-padx=>5,-pady=>5);

my $lbsel=$frame1->Scrolled('Listbox',-scrollbars=>'e',-activestyle=>"dotbox",-width=>40,-height=>30);
$lbsel->configure(-selectmode=>'multiple');
$lbsel->bind('<Double-1>',\&do_lbunselect);

my $bt_geo=$frame1->Button(-text=>"Generate Geo", -command=>\&do_geo);

$lbsel->pack(-fill=>"both", -expand=>1);
$bt_geo->pack(-fill=>"x");

my $frame2=$main->Frame();
$frame2->pack(-side=>"left", -fill=>"both", -expand=>1,-padx=>5,-pady=>5);


my $lbcities=$frame2->Scrolled('Listbox',-scrollbars=>'e',-activestyle=>"dotbox",-width=>40,-height=>30);
$lbcities->configure(-selectmode=>'multiple');

$lbcities->pack(-fill=>"both", -expand=>1);
$lbcities->bind('<Double-1>',\&do_lbselect);

my $frame3=$frame2->Frame();
$frame3->pack(-fill=>"both");

$frame3->Button(-text=>"All", -command=>\&do_sel_all,-state=>'disabled')->pack(-side=>"left",-fill=>"both", -expand=>1);
$frame3->Button(-text=>"Invert", -command=>\&do_sel_invert,-state=>'disabled')->pack(-side=>"left",-fill=>"both", -expand=>1);
$frame3->Button(-text=>"None", -command=>\&do_sel_none,-state=>'disabled')->pack(-side=>"left",-fill=>"both", -expand=>1);

my @files=get_city_list();
my @selected;

refill_list($lbcities, \@files);
refill_list($lbsel, \@selected);

MainLoop();

sub comparator {
	return $a->{$sortmode} cmp $b->{$sortmode};
}

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
	foreach my $sc (@selected){
		print $sc->{city}."\t".$sc->{fname}."\n";
	}
}

sub do_sel_all {
	print "sel_all\n";
}

sub do_sel_invert {
	print "sel_invert\n";
}

sub do_sel_none {
	print "sel_none\n";
}

sub move_record { # index, listsrc, listdest
	my ($idx, $src, $dest) = @_;
	my $item=$src->[$idx];
	splice( @$src, $idx, 1);
	push (@$dest, $item);
}

sub do_lbselect {
	return if $lbcities->size==0;
	print "lbselect\n";
	move_record($lbcities->index('active'), \@files, \@selected);
	refill_list($lbcities, \@files);
	refill_list($lbsel, \@selected);
}

sub do_lbunselect {
	return if $lbsel->size==0;
	print "lbunselect\n";
	move_record($lbsel->index('active'), \@selected, \@files);
	@files = sort comparator @files;
	refill_list($lbcities, \@files);
	refill_list($lbsel, \@selected);
}

sub get_city_list {
	my @inis=glob($path."..\\GeoAM\\geo\\*.ini");
	my @files;
	foreach my $ini (@inis){
		$ini=~/.+\\(.+?)\.ini/is;
		$ini=$1;
		my $inipath="$path..\\GeoAM\\geo\\$ini\\";
		open(INI,"<$inipath$ini.txt") or warn "No file $path$ini.txt\n";
		my @cset=<INI>;
		close(INI);
		my $i=0;
		foreach (@cset){
			my $ci=$_;
			next if $ci=~/\#/is;
			chomp($ci);
			$ci=~/(.+?)\|.+\|(.+)/is;
			my $datapath="$inipath".sprintf('Data%02d.dat', $i++);
			if(-f $datapath){
				push(@files, {city=>$1, state=>$2, fname=>$datapath} );
			}
			else{
				warn "No datafile $datapath\n";
			}
		}
	}
	return sort comparator @files;
}

sub refill_list { # lbox, listref
	$_[0]->delete(0,'end');
	foreach my $fl (@{$_[1]}){
		$_[0]->insert('end',$fl->{city}.', '.$fl->{state});
	}
}