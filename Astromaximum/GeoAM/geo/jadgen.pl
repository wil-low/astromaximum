use strict;

my $rar='d:\Program Files\WinRAR\winrar.exe';
my $jar='..\\..\\GeoAM\\dist\\GeoAM.jar';
my @files;

	$0=~/(.+\\)/is;
	our $path=$1;
	my $manif=$path.'META-INF';
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
	
	open($InF, "<$path$jar") or die "No file";
	binmode($InF);
	my @data=<$InF>;
	$buf=join("",@data);
	close($InF);
	
	open($InF, ">$path$dir\\$fname.jar") or die "No file $dir\\$fname.jar";
	binmode($InF);
		print $InF $buf;
	close($InF);
	
	my $cmd="\"$rar\" f $path$dir\\$fname\.jar $manif";
	print "$cmd\n";
	system($cmd);
	$cmd="\"$rar\" f -ep1 $path$dir\\$fname\.jar $path$dir\\locations.dat";
	print "$cmd\n";
	system($cmd);
	
	my $asize= -s "$dir\\$fname\.jar";
	$jad.="MIDlet-Jar-Size: $asize\n";
	
	open($InF, ">$path$dir\\$fname\.jad") or die "No file";
		print $InF $jad;
	close($InF);
}

