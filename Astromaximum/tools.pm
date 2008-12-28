package tools;

use strict;

our $rar='d:\Program Files\WinRAR\winrar.exe';

our %eventType=qw(EV_VOC 0 EV_SIGN_ENTER 1 EV_ASP_EXACT 2 EV_RISE 3 EV_DEGREE_PASS 4
EV_VIA_COMBUSTA 5 EV_RETROGRADE 6 EV_ECLIPSE 7 EV_TITHI 8 EV_NAKSHATRA 9 EV_SET 10
EV_DECL_EXACT 11 EV_NAVROZ 12 EV_TOP_DAY 13 EV_PLANET_HOUR 14 EV_STATUS 15 EV_SUN_RISE 16
EV_MOON_RISE 17 EV_MOON_MOVE 18 EV_SEL_DEGREES 19 EV_DAY_HOURS 20 EV_NIGHT_HOURS 21
EV_SUN_DAY 22 EV_MOON_DAY 23 EV_TOP_MONTH 24 EV_MOON_PHASE 25 EV_ZODIAC_SIGN 26
EV_PANEL 27 EV_TOPIC_BUTTON 28 EV_DEG_2ND 29 EV_WEEK_GRID 30 EV_MONTH_GRID 31
EV_DECUMBITURE 32 EV_DECUMB_ASPECT 33 EV_DECUMB_BEGIN 34 EV_SUN_DEGREE_LARGE 35
EV_MOON_SIGN_LARGE 36 EV_HELP 37 EV_ASP_EXACT_MOON 38 EV_DEGPASS0 39 EV_DEGPASS1 40
EV_DEGPASS2 41 EV_DEGPASS3 42 EV_HELP0 43 EV_HELP1 44 EV_ASTRORISE 45 EV_ASTROSET 46
EV_APHETICS 47 EV_FAST 48 EV_ASCAPHETICS 49 EV_MSG 50 EV_BACK 51 EV_LAST 52
);
# Any changes above must be synched with constants in Astromaximum/Event.java !!!

sub join_datafiles # year, size, destfile, fname_listref
{
	my $year=shift;
	my $size=$_[0];
	open(OUTF, ">$_[1]") or die "No file $_[1]";
	my @bins=@{$_[2]};
	my @buf;
	my @bodies;
	binmode(OUTF);
	print OUTF pack('n',$year);
	print OUTF pack('n',$#bins+1);
	my $i=0;
	foreach my $ff(@bins){
		open(INF, "<$ff") or die "No file $ff";
		binmode(INF);
		undef $/ ;
		@buf=<INF>;
		close(INF);
		$bodies[$i]="@buf";
		print OUTF pack('n',length($bodies[$i]));
		print(length($bodies[$i])."\n");
		++$i;
		last if $i>=$size;
	}
	foreach my $png(@bodies){
		print OUTF $png;
	}
	close(OUTF);
}

sub writeData # srcfile, destfile, imeichar
{
	my ($src, $dest, $imeichar) = @_;
	open(OUTF, ">>$dest") or die "No file";
	binmode(OUTF);
	open(INF, "<$src") or die "No file";
	binmode(INF);
	undef $/ ;
	my $body=<INF>;
	close(INF);
	if(length($body)>8){
		print OUTF pack('c',$imeichar).$body; #
		print("$src $imeichar\n");
	}
	close(OUTF);
}

sub read_template {
	open(INF, "<template.jad") or die "No file";
	my @data=<INF>;
	close(INf);
	my $template=join("",@data);
	return \$template;
}

sub create_geo { # code, region, descript, destdir, locationpath, is_numbered, year, citiesref, templatedataref
	my ($prefix, $reg, $desc, $destdir, $locpath, $is_numbered, $year, $citiesref, $template )=@_;
	my $code='';
	if(!$template){
		$template=read_template();
	}
	my $jad=$$template;
	if($is_numbered){
		my $locsz= -s '.temp/locations.dat';
		$locsz=~/(\d{0,4})$/is;
		$code="-$1";
		warn $locsz;
	}
	$year=~s/\d\d(\d\d)/$1/is;
	my $fname="$prefix\'$year$code";
	$jad=~s/<YEAR>/$year/isg;
	$jad=~s/<REGION>/$reg/isg;
	$jad=~s/<CODE>/$code/isg;
	$jad=~s/<DESC>/$desc/isg;
	$jad=~s/<JAR>/$fname\.jar/isg;
	my $i=0;
	foreach my $lname (@{$citiesref}){
		$jad.="Loc$i: $lname\n";
		$i++;
	}
#	die $jad;
	mkdir ".temp/META-INF/" unless -d ".temp/META-INF/";
	open(INF, ">.temp/META-INF/MANIFEST.MF") or die "No file";
		print INF $jad;
	close(INF);
	
	$year=~/\d(\d)/is;
	open(INF, "<images/geo/$1.png");
	binmode(INF);
	my @data=<INF>;
	close(INF);
	open(OUTF, ">.temp/icon.png");
	binmode(OUTF);
	print OUTF join("",@data);
	close(OUTF);

	open(INF, "<GeoAM/dist/GeoAM.jar") or die "No file GeoAM/dist/GeoAM.jar";
	binmode(INF);
	@data=<INF>;
	close(INF);
	
	open(INF, ">$destdir$fname.jar");
	binmode(INF);
		print INF join("",@data);
	close(INF);

	my $cmd="\"$rar\" a -r -ep1 $destdir$fname\.jar .temp/*";
	print("$cmd\n");
	system($cmd);
	
	my $asize= -s "$destdir$fname\.jar";
	$jad.="MIDlet-Jar-Size: $asize\n";
	
	open(INF, ">$destdir$fname\.jad") or die "No file";
		print INF $jad;
	close(INF);
	return $code;
}

sub get_year { 
# in: path
# out: year, day count
	open(FYEAR, "<$_[0]".'year.txt') or die "No file: $!";
	my $year=<FYEAR>;
	close(FYEAR);
	chomp($year);
	return ($year, day_count($year));
}

sub day_count{
	my $dc=365;
	my $yr=shift;
	if($yr%4==0){
		$dc++;
	}
	my $rem=$yr-int($yr/100)*100;
	$yr=int($yr/100);
	if($rem==0 and $yr%4>0){
		$dc--;
	}
	return $dc;
}


1;
