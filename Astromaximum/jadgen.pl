use strict;
use tools;

my $jar='GeoAM\\dist\\GeoAM.jar';
my @files;

	$0=~/(.+\\)/is;
	our $mypath=$1;
	our $path=$mypath.'GeoAM\\geo\\';
	my $manif=$path.'META-INF';
	our $deploy=$mypath.'GeoAM\\deploy\\';
	mkdir $deploy unless -d $deploy;
	
	mkdir $manif unless -d $manif;
	$manif.="\\MANIFEST.MF";
	
if($#ARGV==0){
	$files[0]=".\\$ARGV[0].ini";
}
else{
	@files=glob("$path*.ini");
}
my $InF=undef;
my $template=tools::read_template();

foreach my $city_inf(@files){
	$city_inf=~/.+\\(.+?)\.ini/is; # файл со списком городов
	$city_inf=$1;
	my $dir=$city_inf;
	my $buf;
	my $fname="GeoAM-$city_inf";
	
	open($InF, "<$path$city_inf.ini") or die "No file";
	my @data=<$InF>;
	$buf=$data[0];
	close($InF);
	chomp($buf);
	my($reg, $code)=$buf=~/\#\#(.+):(.+)/is;
	tools::create_geo($city_inf, $code, $reg, $deploy, "$path$city_inf", $template);
}

