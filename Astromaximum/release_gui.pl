use strict;
use Tk;
use warnings;
use lib 'D:/Willow/prj/astrology/nomad_prj'; 
use lib 'd:/projects/nomad_prj';
use tools;


my %imeis=qw( 
	sonnenturm 359593001109710 
	moto 11234564 
	igor 359315001137255 
);

my $init_imei=$imeis{'sonnenturm'};

$init_imei=$imeis{$ARGV[0]} if defined $ARGV[0];
$init_imei='0' x (15-length($init_imei)).$init_imei;

our $year='';
our $antpath='d:\\Program Files\\netbeans-5.5\\ide7\\ant\\bin\\ant.bat';
if(! -f $antpath){
	$antpath='d:\\netbeans-5.5\\ide7\\ant\\bin\\ant.bat';
}
my @chks;
my ($is_logger)=(0);

$0=~/(.+\\)/is;
our $path=$1;
our $sortmode='city';

($year)=tools::get_year($path);
die "Invalid year record: $year" if $year!~/\d{4}/is;
 
my @files=get_city_list();
my @selected;
my $debug=0;
my $kzip=1;
my $ltime=0;

my $main = new MainWindow(-title=>"Astromaximum Release GUI - $year");
my $frame0=$main->Frame();
my $imei=$frame0->Entry(-text=>$init_imei);
my $bt_imei=$frame0->Button(-text=>"IMEI", -command=>[\&do_imei,'midp2y2007release']);
my $bt_imei_logger=$frame0->Button(-text=>"IMEI logger", -command=>[\&do_imei,'midp2y2007release_logger']);
my $lbox = $frame0->Entry(-text=>'0');
my $bt_timebomb=$frame0->Button(-text=>"Time Bomb", -command=>\&do_timebomb);

$imei->pack();
$bt_imei->pack(-fill=>"x");
$bt_imei_logger->pack(-fill=>"x",-pady=>3);

my $frame6=$frame0->Frame();
$frame6->Checkbutton(-text=>"Debug", -variable=>\$debug)->pack(-side=>'left');
$frame6->Checkbutton(-text=>"Kzip", -variable=>\$kzip)->pack(-side=>'left');
$frame6->pack(-pady=>3);
$lbox->pack();

$bt_timebomb->pack(-fill=>"x");
#$frame0->Checkbutton(-text=>"Localtime", -variable=>\$ltime)->pack();

my $frame4=$frame0->Frame();

our $lbsize=$frame4->Label();


my $frame5=$frame4->Frame();
$frame5->Label(-text=>"Sort by:")->pack(-side=>'left');
$frame5->Radiobutton(-text=>'city', -value=>'city', -command=>\&do_sort, -variable=>\$sortmode)->pack(-side=>'left');
$frame5->Radiobutton(-text=>'state', -value=>'state', -command=>\&do_sort, -variable=>\$sortmode)->pack(-side=>'left');

#$year=~/20(\d\d)/is;
my $geofile=$frame4->Entry(-text=>"Cities");
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
#$lbcities->configure(-selectmode=>'multiple');

$lbcities->pack(-fill=>"both", -expand=>1);
$lbcities->bind('<Double-1>',\&do_lbselect);


$frame5->pack(-fill=>"x", -pady=>5, -side=>'bottom');

$lbsize->pack(-fill=>"x", -pady=>5, -side=>'bottom');

$frame0->pack(-side=>"left",-fill=>"both", -expand=>1,-padx=>5,-pady=>5);

$frame1->pack(-side=>"left", -fill=>"both", -expand=>1,-pady=>5);

#$lbsel->configure(-selectmode=>'multiple');
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
	my $cmd="\"$antpath\" -f Astromaximum\\build.xml -Dconfig.active=$conf -Dfile.reference.Astromaximum-deploy=./deploy -Dtb.timeout=$timeout -Dconfigs.$conf.debug.level=";
	if($debug){
		$cmd.="debug";
	}
	else{
		$cmd.="fatal";
	}
  $cmd.=' -Dneed.kzip=true' if $kzip;
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
	$code.='0' x (15-length($code));
	if($code=~/\A\d{15}\Z/is){
		print "imei\n";
		my $cmd="\"$antpath\" -f Astromaximum\\build.xml -Dconfig.active=$conf -Dfile.reference.Astromaximum-deploy=./deploy -Dimei.code=$code -Dconfigs.$conf.debug.level=";
		if($debug){
			$cmd.="debug";
		}
		else{
			$cmd.="fatal";
		}
		$cmd.=' -Dneed.kzip=true' if $kzip;
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
		my $code=tools::create_geo("Cities", $geocap, $geod, "GeoAM\\deploy\\", ".temp\\", 1, $year);
		$main->messageBox(-icon => 'info', -message => "Geo build #$code successful", -title => 'Message', -type => 'Ok');
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
	refresh_lbsize();
}

sub do_lbunselect {
	return if $lbsel->size==0;
	move_record($lbsel->index('active'), \@selected, \@files);
	do_sort();
	refill_list($lbsel, \@selected);
	refresh_lbsize();
}

sub get_city_list {
	my @inis=glob($path."GeoAM\\geo\\*.ini");
	my @files;
	foreach my $ini (@inis){
		$ini=~/.+\\(.+?)\.ini/is;
		$ini=$1;
		my $inipath=$path."GeoAM\\geo\\$ini\\";
		if(open(INI,"<$inipath$ini.txt")){
			my @cset=<INI>;
			close(INI);
			my $i=0;
			foreach (@cset){
				my $ci=$_;
				next if $ci=~/\#/is;
				chomp($ci);
				$ci=~s/\A\s*\"(.+)\"\s*\Z/$1/is;
				$ci=~/(.+?)\|.+\|(.+)/is;
				my($city,$state)=($1,$2);
				$city=~s/.+!//is;
				$state=~s/.+\$//is;
				my $datapath="$inipath".sprintf('Data%02d.dat', $i++);
				if(-f $datapath){
					push(@files, {city=>$city, state=>$state, fname=>$datapath} );
				}
				else{
					warn "No datafile $datapath\n";
				}
			}
		}
		else{
			 warn "No file $path$ini.txt\n";
		}
	}
	return sort comparator @files;
}

sub refill_list { # lbox, listref
	$_[0]->delete(0,'end');
	foreach my $fl (@{$_[1]}){
		if($sortmode eq 'city'){
			$_[0]->insert('end',$fl->{city}.', '.$fl->{state});
		}
		else{
			$_[0]->insert('end',$fl->{state}.', '.$fl->{city});
		}
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

sub refresh_lbsize {
	if($#selected==-1){
		$lbsize->configure('-text'=>'');
	}
	else{
		my $locsize=(5100+($#selected+1)*8300/2.25)/1024;
		$lbsize->configure('-text'=>sprintf('Cities = %d,   file size ~ %d kb',$#selected+1, $locsize));
	}
}
