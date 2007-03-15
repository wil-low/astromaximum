use strict;

my $rar='d:\Program Files\WinRAR\winrar.exe';
my $jar='dist\\GeoAM.jar';
my @files;

	$0=~/(.+\\)/is;
	our $mypath=$1;
	our $path=$mypath.'geo\\';
	my $manif=$path.'META-INF';
	our $deploy=$mypath.'deploy\\';
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
my $template;
open($InF, "<$path\\template.jad") or die "No file";
my @data=<$InF>;
$template=join("",@data);
close($InF);

foreach my $city_inf(@files){
	$city_inf=~/.+\\(.+?)\.ini/is; # файл со списком городов
	$city_inf=$1;
	
	
	my $dir=$city_inf;
	
	our $outbuf;
	our $fname;
	my $buf;
	my $fname="GeoAM-$city_inf";
	
	open($InF, "<$path\\$city_inf.ini") or die "No file";
	@data=<$InF>;
	$buf=$data[0];
	close($InF);
	chomp($buf);
	my($reg, $code)=$buf=~/\#\#(.+):(.+)/is;
	my $jad=$template;
	$jad=~s/<REGION>/$reg/sg;
	$jad=~s/<CODE>/$code/s;
	$jad=~s/<JAR>/$fname\.jar/is;
	open($InF, ">$manif") or die "No file";
		print $InF $jad;
	close($InF);
	
	open($InF, "<$mypath$jar") or die "No file";
	binmode($InF);
	my @data=<$InF>;
	$buf=join("",@data);
	close($InF);
	
	open($InF, ">$deploy$fname.jar") or die "No file $deploy$fname.jar";
	binmode($InF);
		print $InF $buf;
	close($InF);
	
	my $cmd="\"$rar\" f $deploy$fname\.jar $manif";
	print "$cmd\n";
	system($cmd);
	$cmd="\"$rar\" a -ep1 $deploy$fname\.jar $path$dir\\locations.dat";
	print "$cmd\n";
	system($cmd);
	
	my $asize= -s "$deploy$fname\.jar";
	$jad.="MIDlet-Jar-Size: $asize\n";
	
	open($InF, ">$deploy$fname\.jad") or die "No file";
		print $InF $jad;
	close($InF);
}

