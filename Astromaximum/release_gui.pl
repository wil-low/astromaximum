use strict;
use Tk;
use warnings;
use lib 'D:/Willow/prj/astrology/nomad_prj/'; 
use lib 'd:/projects/nomad_prj';
use tools;

our $antpath='d:\\Program Files\\netbeans-5.5\\ide7\\ant\\bin\\ant.bat';
if(! -f $antpath){
	$antpath='d:\\netbeans-5.5\\ide7\\ant\\bin\\ant.bat';
}
my @chks;
my ($is_logger)=(0);

$0=~/(.+\\)/is;
our $path=$1;
our $sortmode='city';

my @files=get_city_list();
my @selected;
my $debug=0;
my $ltime=0;

my $main = new MainWindow(-title=>"Astromaximum Release GUI");
my $frame0=$main->Frame();
my $imei=$frame0->Entry(-text=>'359593001109710');
my $bt_imei=$frame0->Button(-text=>"IMEI", -command=>[\&do_imei,'midp2y2007release']);
my $bt_imei_logger=$frame0->Button(-text=>"IMEI logger", -command=>[\&do_imei,'midp2y2007release_logger']);
my $lbox = $frame0->Entry(-text=>'0');
my $bt_timebomb=$frame0->Button(-text=>"Time Bomb", -command=>\&do_timebomb);

$imei->pack();
$bt_imei->pack(-fill=>"x");
$bt_imei_logger->pack(-fill=>"x",-pady=>3);

$frame0->Checkbutton(-text=>"Debug", -variable=>\$debug)->pack(-pady=>10);
$lbox->pack();

$bt_timebomb->pack(-fill=>"x");
#$frame0->Checkbutton(-text=>"Localtime", -variable=>\$ltime)->pack();

my $frame4=$frame0->Frame();

my $frame5=$frame4->Frame();
$frame5->Label(-text=>"Sort by:")->pack(-side=>'left');
$frame5->Radiobutton(-text=>'city', -value=>'city', -command=>\&do_sort, -variable=>\$sortmode)->pack(-side=>'left');
$frame5->Radiobutton(-text=>'state', -value=>'state', -command=>\&do_sort, -variable=>\$sortmode)->pack(-side=>'left');

my $geofile=$frame4->Entry(-text=>'Cities');
my $geodesc=$frame4->Entry(-text=>'<No description>');
$frame4->pack(-fill=>"both", -expand=>1,-padx=>5, -side=>'bottom');
$geodesc->pack(-side=>'bottom');
$frame4->Label(-text=>"Midlet description:")->pack(-side=>'bottom');

$frame4->pack(-fill=>"both", -expand=>1,-padx=>5, -side=>'bottom');
$geofile->pack(-side=>'bottom');
$frame4->Label(-text=>"Midlet name:")->pack(-side=>'bottom');

my $frame1=$main->Frame();
my $frame2=$main->Frame();

my $lbsel=$frame1->Scrolled('Listbox',-scrollbars=>'e',-activestyle=>"dotbox",-width=>40,-height=>30);
my $bt_geo=$frame1->Button(-text=>"Generate Geo", -command=>\&do_geo);
my $lbcities=$frame2->Scrolled('Listbox',-scrollbars=>'e',-activestyle=>"dotbox",-width=>40,-height=>30);
$lbcities->configure(-selectmode=>'multiple');

$lbcities->pack(-fill=>"both", -expand=>1);
$lbcities->bind('<Double-1>',\&do_lbselect);


$frame5->pack(-fill=>"x", -pady=>5, -side=>'bottom');

$frame0->pack(-side=>"left",-fill=>"both", -expand=>1,-padx=>5,-pady=>5);

$frame1->pack(-side=>"left", -fill=>"both", -expand=>1,-pady=>5);

$lbsel->configure(-selectmode=>'multiple');
$lbsel->bind('<Double-1>',\&do_lbunselect);


$lbsel->pack(-fill=>"both", -expand=>1);
$bt_geo->pack(-fill=>"x");

$frame2->pack(-side=>"left", -fill=>"both", -expand=>1,-padx=>5,-pady=>5);

my $frame3=$frame2->Frame();
$frame3->pack(-fill=>"both");

$frame3->Button(-text=>"All", -command=>\&do_sel_all,-state=>'disabled')->pack(-side=>"left",-fill=>"both", -expand=>1);
$frame3->Button(-text=>"Invert", -command=>\&do_sel_invert,-state=>'disabled')->pack(-side=>"left",-fill=>"both", -expand=>1);
$frame3->Button(-text=>"None", -command=>\&do_sel_none,-state=>'disabled')->pack(-side=>"left",-fill=>"both", -expand=>1);


refill_list($lbcities, \@files);
refill_list($lbsel, \@selected);

MainLoop();

sub comparator {
	return $a->{$sortmode} cmp $b->{$sortmode};
}

sub do_timebomb {
	print "timebomb\n";
	my $conf='midp2y2007release_tb';
	my $timeout=$lbox->get();
	my $cmd="\"$antpath\" -f Astromaximum\\build.xml -Dconfig.active=$conf -Dtb.timeout=$timeout -Dconfigs.$conf.debug.level=";
	if($debug){
		$cmd.="debug";
	}
	else{
		$cmd.="fatal";
	}
	$cmd.=" clean deploy";
	print "$cmd\n";
	my $res=system($cmd);
	if($res==0){
		$main->messageBox(-icon => 'info', -message => "Build successful", -title => 'Message', -type => 'Ok');
		return;
	}
	$main->messageBox(-icon => 'error', -message => "Build failed!", -title => 'Message', -type => 'Ok');
}

sub do_imei { # config_name
	my $conf=shift;
	my $code=$imei->get();
	if($code=~/\A\d{15}\Z/is){
		print "imei\n";
		my $cmd="\"$antpath\" -f Astromaximum\\build.xml -Dconfig.active=$conf -Dimei.code=$code -Dconfigs.$conf.debug.level=";
		if($debug){
			$cmd.="debug";
		}
		else{
			$cmd.="fatal";
		}
		$cmd.=" clean deploy";
		print "$cmd\n";
		my $res=system($cmd);
		if($res==0){
			$main->messageBox(-icon => 'info', -message => "Build successful", -title => 'Message', -type => 'Ok');
			return;
		}
	}
	$main->messageBox(-icon => 'error', -message => "Build failed!", -title => 'Message', -type => 'Ok');
}

sub do_geo {
	my $geocap=$geofile->get();
	my $geod=$geodesc->get();
	if($geocap and $geod and $#selected>=0){
		print "geo\n";
		my @geo;
		foreach my $sc (@selected){
			print $sc->{city}."\t".$sc->{fname}."\n";
			push(@geo, $sc->{fname});
		}
		mkdir '.temp' unless -d '.temp';
		tools::join_datafiles($#selected+1, $path.".temp\\locations.dat", \@geo);
		tools::create_geo("USER", $geocap, $geod, "GeoAM\\deploy\\", ".temp\\");
		$main->messageBox(-icon => 'info', -message => "Geo build successful", -title => 'Message', -type => 'Ok');
		return;		
	}
	$main->messageBox(-icon => 'error', -message => "Geo build failed!", -title => 'Message', -type => 'Ok');
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
	move_record($lbcities->index('active'), \@files, \@selected);
	my $idx=$lbcities->nearest(0);
	refill_list($lbcities, \@files);
	$lbcities->yview($idx);
	refill_list($lbsel, \@selected);
}

sub do_lbunselect {
	return if $lbsel->size==0;
	move_record($lbsel->index('active'), \@selected, \@files);
	do_sort();
	refill_list($lbsel, \@selected);
}

sub get_city_list {
	my @inis=glob($path."GeoAM\\geo\\*.ini");
	my @files;
	foreach my $ini (@inis){
		$ini=~/.+\\(.+?)\.ini/is;
		$ini=$1;
		my $inipath=$path."GeoAM\\geo\\$ini\\";
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

sub do_sort {
	@files = sort comparator @files;
	refill_list($lbcities, \@files);
}

sub get_abilities { # config
	my $conf=shift();
	if($conf){
		$conf='configs\.'.$conf.'\.abilities';
	}
	else{
		$conf='abilities';
	}
	open(PROP, "<$path".'Astromaximum\\nbproject\\project.properties') or die 'No file';
	my @lines=<PROP>;
	close(PROP);
	@lines=grep(/$conf/, @lines);
	chomp($lines[0]);
	$lines[0]=~s/(.+?)(abilities)/$2/is;
	return $lines[0];
}