#!/usr/bin/perl
package main;
use strict;
#use warnings;
#use diagnostics;
use POSIX;
my $islocal=$0=~/\.pl$/is;
my $done=0;
if(!$islocal){
	$const::DIR_TEMPLATE='source';
}
my $nb_user='$HOME/.netbeans/6.7';
my $platform='$HOME/wtk251';
our $winda=$^O=~/Win/is;
our $ext = '';
if($winda){
	$nb_user='%USERPROFILE%/.netbeans/6.7';
	$platform='D:\WTK251';
	$ext = '.exe';
}

our $year_info;
#$year_info="инфо года";

our $path;
our $file_sign="\x50\x4B\x03\x04";
our $fdir_sign="\x50\x4B\x01\x02";
our $PREFIX='Cities';
our $GeoAMclass='GeoInstaller';

our %eventFlags=qw(EF_PLANET1 2 EF_PLANET2 4 EF_DEGREE 8 EF_SHORT_DEGREE 64);
our $output=''; our $paramcount=0; our $outbuf; our $errors=0;
our %hash;


$0=~/(.+)[\\\/]/is;
$path=$1;
while ($path=~s/\/\.\.$//) {
    $path=~s/\/[^\/]+$//;
}
if(!$path){
	#	if($winda){
	$path='.';
	#  }
	#  else{
	#  	$path=`sh pwd`;
	#  }
}
chomp($path);

my $pwdgen_local=$path.'/../../pwdgen_local.php';

my $jar_path=$path;

open(FLOG, ">$path/gen_amax.log");

echo("Command line:  $0 @ARGV\n");
if($islocal){
	require 'genconst.pm';
	if(!scalar(@ARGV)){
		print "This script generates ready-to-use $const::PRODUCT $const::VERSION distribution.\n";
		print "Parameters:\n";
		print "\t<config>: [rebuild|notest|imei|tb|demo|microemu|join|lang|amtext|cleanup]\n";
		print "\t<year>\n";
		print "\t<lang>\n";
		print "\t<loclist file>\n";
		print "\t<output jar>, or '-' for default\n";
		print "\t[imei|timebomb|tb_timeout]\n";
		print "\t[messjar] (dangerous, can make archive unreadable)\n";
		exit(1);
	}

	require 'tools.pm';
	require 'Crc32.pm';
	die '$const::DIR_TEMP' unless $const::DIR_TEMP;
	rm_all("$path/$const::DIR_TEMP");
}
else{    # site has different folder structure
	my $id;
	($const::DIR_TEMP, $id)=random('source');
	$jar_path.='/../';
	$const::DIR_INTERPRET='source/interpret';
	$const::DIR_IMG='source/icons';
}

our $config=shift(@ARGV);
our $year=shift(@ARGV);
our $lang=shift(@ARGV);
our $loclist=shift(@ARGV);
our $outfile=shift(@ARGV);

if($islocal and ($config eq 'join')){
	inject_common($year, "$path/$year.comm");
	exit(0);
}

if($islocal and ($config eq 'lang')){
	inject_lang($lang);
	exit(0);
}

if($islocal and ($config eq 'rebuild')){
	echo("Rebuilding all configs...\n");
	my $antpath;
	my @app=(
	'/home/willow/program/nb67/java2/ant/bin/ant',
	'd:/Program Files/nb67/java2/ant/bin/ant.bat',
	);
	foreach (@app){
		if(-f $_){
			$antpath=$_;
			last;
		}
	}
	if(!$antpath){
		mydie("ANT not found in this system!!!\n");
	}
	$antpath="\"$antpath\"";
	$antpath="sh $antpath" unless $antpath=~/\.bat\"$/;
	
	echo("\n--------------------------------\n");
	echo("--- Config $_ ---\n");
	echo("--------------------------------\n");
	my $cmd="$antpath -quiet -f geoLib/build.xml -Dnetbeans.user=\"$nb_user\" ".
		"-Dplatform.home=\"$platform\" clean jar";
	echo("$cmd\n");
	mydie("BUILD ERROR") if system($cmd);
	my @conf=('tb', 'demo', 'imei', 'microemu'
#	'notest', 'notest_logger', 'imei', 'tb_logger'
	);
	my @tmpl=glob("$path/templates/*.jar");
	foreach(@tmpl){
	    unlink $_;
	}
	foreach(@conf){
		echo("\n--------------------------------\n");
		echo("--- Config $_ ---\n");
		echo("--------------------------------\n");
		my $cmd="$antpath -quiet -f Astromaximum/build.xml -Dconfig.active=$_ ".
			"-Dconfigs.$_.javac.debug=false -Dconfigs.$_.obfuscation.level=9 ".
			"-Dconfigs.$_.javac.optimize=true -Drebuild.only=true -Dnetbeans.user=\"$nb_user\" ".
			"-Dplatform.home=\"$platform\" -Dproject.geoLib=\"$path/../geoLib\" clean jar";
		echo("$cmd\n");
		mydie("BUILD ERROR") if system($cmd);
	}
	my $conf="DefaultConfiguration";
	echo("\n--------------------------------\n");
	echo("--- Config geo: $conf ---\n");
	echo("--------------------------------\n");
	$cmd="$antpath -quiet -f GeoAM/build.xml -Dcjavac.debug=false -Djavac.optimize=true ".
		"-Drebuild.only=true -Dnetbeans.user=\"$nb_user\" -Dplatform.home=\"$platform\" ".
		"-Dproject.geoLib=\"$path/../geoLib\" clean jar";
	echo("$cmd\n");
	mydie("BUILD ERROR") if system($cmd);
	exit(0);
}
our $messjar=0;

my $argv="@ARGV";
$messjar=1 if $argv=~/messjar/is;

if($config=~/amtext/is){
	unzip("$path/$const::DIR_TEMPLATE/AMtext.jar");
	inject_lang($lang, 'amtext');
	inject_icon('amtext', "res/");

	my @files=glob("$path/$const::DIR_TEMP/*");
	my $out='';
	foreach my $fn(@files){
		next if $fn!~/[\\\/](\d+)$/is;
		$out.=pack("C", $1);
	}
	open(OUT, ">$path/$const::DIR_TEMP/index") or mydie("$!: $path/$const::DIR_TEMP/index");
	binmode(OUT);
	print(OUT $out);
	close(OUT);
	#	$outfile=~s/\.jar/-$lang.jar/is;
	do_jar('AMtext', 'AMtext', $outfile, "TextInstaller");
	$done=1;
}
else{
	mydie("Invalid year '$year'") if $year!~/^\d{4}$/is;
}

if($islocal){
	$loclist=ensure_slash($loclist);
	mydie("Invalid loclist '$loclist'") if ! -f $loclist;
}
my $dest='';

if($config=~/demo/is){
	unless($outfile=~/r$/is){
		$outfile="$path/Astromaximum/deploy/$const::PRODUCT".'Demo.jar' ;
	}
}

$year=~/\d\d(\d\d)/is;
my $ye=$1;

my $ofs=shift(@ARGV);
$ofs=-24 unless $ofs;
my $delta=shift(@ARGV);
$delta=2880 unless $delta;

if($config=~/geo-$/is){
	unless($outfile=~/r$/is){
		$outfile="$path/Astromaximum/deploy/$ye"."_Geo.jar";
	}
}

my $ye_prod=$ye.$const::PRODUCT;
my $suite="$const::PRODUCT$ye";

if($outfile eq '-'){
	$outfile="$path/Astromaximum/deploy/$ye_prod.jar" ;
	echo("Outfile is '-', setting to $outfile\n");
}
$outfile=ensure_slash($outfile);
echo("Processing <$config> for $year lang=$lang using locations from $loclist...\n");


if($config=~/tb/is){
	if($config=~/logger/is){
		unzip("$path/$const::DIR_TEMPLATE/Astromaximum-tb-logger.jar");
	}
	else{
		unzip("$path/$const::DIR_TEMPLATE/Astromaximum-tb.jar");
	}
	inject_lang($lang);
	inject_amdata();
	do_timebomb($ofs, $delta);
	inject_common($year, "$path/$const::DIR_TEMP/common.dat");
	inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
	inject_icon('a', "res/");
	do_jar($suite, $ye_prod, $outfile, $const::PRODUCT);
	$done=1;
	do_messjar($outfile);
}

if($config=~/demo/is){
	#    $year=2006;
	unzip("$path/$const::DIR_TEMPLATE/AstromaximumDemo.jar");
	inject_lang($lang, 'demo');
	inject_amdata();
	do_timebomb($ofs, $delta);
	inject_common($year, "$path/$const::DIR_TEMP/c.dat");
	inject_locations($year, $loclist, "$path/$const::DIR_TEMP/l.dat");
	inject_icon('a', "res/");
	do_jar($suite, $ye_prod, $outfile, $const::PRODUCT);
	do_messjar($outfile);
	$done=1;
}

if($config=~/geo-$/is){
	unzip("$path/$const::DIR_TEMPLATE/GeoAM.jar");
	my $locname=inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
	inject_icon('');
	do_jar($locname, $locname, $outfile, $GeoAMclass);
	do_messjar($outfile);
	$done=1;
}

if($islocal){
    my $com_file = "$path/$const::DIR_TEMP/common.dat";
    my $loc_file = "$path/$const::DIR_TEMP/locations.dat";

    if($config=~/(notest|freetest|microemu)$/is){
		unzip("$path/$const::DIR_TEMPLATE/Astromaximum-$config.jar");
		inject_lang($lang);
        inject_common($year, $com_file);
        inject_locations($year, $loclist, $loc_file);
        if($config eq 'microemu') {
            datafile_install('COMMON_DAT', $com_file);
            datafile_install('LOCATIONS_DAT', $loc_file);
            unlink($com_file);
            unlink($loc_file);
        }
		inject_icon('a', "res/");
		do_jar($suite, $ye_prod, $outfile, $const::PRODUCT);
		do_messjar($outfile);
		$done=1;
	}

	if($config=~/imei$/is){
		my $imei=shift(@ARGV);
		$imei='0' x 15 unless $imei;
		unzip("$path/$const::DIR_TEMPLATE/Astromaximum.jar");
		inject_lang($lang);
		inject_common($year, "$path/$const::DIR_TEMP/common.dat", $imei);
		inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
		inject_amdata();
		inject_icon('a', "res/");
		do_jar($suite, $ye_prod, $outfile, $const::PRODUCT);
		do_messjar($outfile);
		$done=1;
	}

	if($config=~/imei_logger$/is){
		my $imei=shift(@ARGV);
		$imei='0' x 15 unless $imei;
		unzip("$path/$const::DIR_TEMPLATE/Astromaximum-logger.jar");
		inject_lang($lang);
		inject_common($year, "$path/$const::DIR_TEMP/common.dat", $imei);
		inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
		inject_amdata();
		inject_icon('a', "res/");
		do_jar($suite, $ye_prod, $outfile, $const::PRODUCT);
		do_messjar($outfile);
		$done=1;
	}
}
mydie("Invalid config <$config>") if !$done;
echo("\n--- $outfile ---\n");
rm_all("$path/$const::DIR_TEMP");
exit(0);

sub ensure_slash{
	$_[0]=~s/\//\\/isg if $winda;
	return $_[0];
}


sub copy_file{
	open(INF,"<$_[0]") or mydie("Cannot open file $_[0]: $!");
	binmode(INF);
	my @body=<INF>;
	close (INF);
	open(OUTF,">$_[1]") or mydie("Cannot open file $_[1]: $!");
	binmode(OUTF);
	print OUTF join('', @body);
	close (OutF);
	echo("cp-> $_[1]\n");
}

sub inject_amdata{
	return;
	copy_file("$path/Astromaximum/src/Amdata.class", "$path/$const::DIR_TEMP/Amdata.class");
	open(INF,"<$path/Astromaximum/src/Amdata.class") or mydie("Cannot open file");
	binmode(INF);
	my @body=<INF>;
	close (INF);
	open(OUTF,">$path/$const::DIR_TEMP/Amdata.class") or mydie("Cannot open file");
	binmode(OUTF);
	print OUTF join('', @body);
	close (OutF);
}

sub inject_icon{ # prefix, subdir
	$ye=~/(\d)$/is;
	my $prefix=shift;
    my $fn = ">$path/$const::DIR_TEMP/$_[0]"."icon.png";
	open(INF, $fn) or mydie("Cannot open $fn");
	binmode(INF);
	my @body=<INF>;
	close (INF);
    $fn = ">$path/$const::DIR_TEMP/$_[0]"."icon.png";
	open(OUTF, $fn) or mydie("Cannot open $fn");
	binmode(OUTF);
	print OUTF join('', @body);
	close (OutF);
	echo("$const::DIR_TEMP/$_[0]"."icon.png written from $path/$const::DIR_IMG/$prefix$1.png\n");
}

sub dbconnect{
	require DBI;
	require CGI;
	my($DB_SERVER,$DB_NAME,$DB_PORT,$DB_SUPERUSER,$DB_SUPERUSER_PWD,$DB_USER,$DB_USER_PWD);

	if(-f $pwdgen_local){ # local=true
		$DB_SERVER='localhost';
		$DB_NAME='amax';
		$DB_PORT='3306';

		$DB_SUPERUSER='root';
		$DB_SUPERUSER_PWD='toor';
		$DB_USER='user';
		$DB_USER_PWD='user';
	}
	else{
		$DB_SERVER='mysql300.1gb.ua';
		$DB_NAME='gbua_x_astroc8a';
		$DB_PORT='3306';

		$DB_SUPERUSER='gbua_x_astroc8a';
		$DB_SUPERUSER_PWD='f3cf9cc0';
		$DB_USER='user';
		$DB_USER_PWD='user';
	}
	my $dsn = "DBI:mysql:database=$DB_NAME;host=$DB_SERVER";
	echo("DSN: $dsn\n");
	return DBI->connect($dsn, $DB_SUPERUSER, $DB_SUPERUSER_PWD);
}

sub inject_locations{
	my($locname, $locnum);
	if($islocal){
		if(!$_[0]){
			mydie("Usage: inject_locations.pl <year> <city list> <dest file>\n");
		}
		my @fn;
		open(IN, "<$_[1]") or mydie("error $!: $_[1]\n");
		while(my $ln=<IN>){
			if($ln=~/(\w+):(Data\d+)\s+(.+)/is){
				my $fn="$path/data/archive/$_[0]/$1/$2.dat";
				if(!$locname){
					$locname=$3;
				}
				my $dfile=ensure_slash($fn);
				push(@fn, $dfile);
				$locnum++;
			}
		}
		close(IN);
		my $i=scalar(@fn);
		tools::join_datafiles($_[0], $i, $_[2], \@fn);
	}
	else{
		my($year, $ids, $outf)=@_;
		my $dbh=dbconnect();
		mydie("Can't dbconnect()") unless $dbh;
		$ids=~s/^,+//is;
		$ids=~s/,+$//is;
		my $stat=sprintf("SELECT DISTINCT cities.name, data FROM cities, locations ".
		"WHERE cities.id IN (%s) AND city_id=cities.id AND year=%s".
		" ORDER BY cities.name",$ids,$year);
		echo("$stat\n");
		my $sth = $dbh->prepare($stat);
		$sth->execute || mydie($dbh->errstr);
		my @iids=split(/,/is, $ids);
		my @data;
		$locnum=$sth->rows;
		mydie("Not enough locations for $year: '$ids' = $locnum") if $locnum!=scalar(@iids);
		while(my @row = $sth->fetchrow_array){
			push(@data, $row[1]);
			if(!$locname){
				$locname=$row[0];
			}
		}
		$sth->finish;
		$dbh->disconnect;
		join_datafiles2($_[0], $outf, \@data);
	}
	echo("$_[2] written\n");
	$locname='Geo' if $locnum>1;
	$locname=~s/[\n\r]//sg;
	return $ye.$locname;
}

sub inject_common{
	if($islocal){
		my $imei='000000000000000';
		#our $imei='359308007701623';
		#die sprintf('%x',substr($imei,0,8));
		if(scalar(@_)==0){
			mydie("Usage: <year> [dest dir] [IMEI]\n");
		}
		my ($year,$month, $day, $hour, $min, $day_count)=($_[0],1,1,0,0,365);
		if($year%100==0){
			if($year%400==0){
				$day_count++;
			}
		}
		else{
			if($year%4==0){
				$day_count++;
			}
		}


		if($_[2]){
			if($_[2]=~/^\d{15}$/is){
				$imei=$_[2];
			}
			else{
				echo("Invalid IMEI=$_[2],using $imei\n");
			}
		}
		$dest=$_[1] if $_[1];
	        my $header=pack('nCCnna*',$year, $month, $day, length($year_info), $day_count, $year_info);
		my $path1=$path;

		$path1.="/data/archive/$year";
		mydie("No dir $path1") unless -d $path1;
		open(OUTF, ">$dest") or mydie("$! $dest");
		binmode(OUTF);
		print OUTF $header;
		close(OUTF);

		my @bins=glob("$path1/*.bin");
		#my @bins=glob("$path/retro09.bin");
		my $counter=0;
		foreach my $ff(@bins){
			if($ff=~/(rise|set|navroz|geo|nakshatra|degall|aphetics)/is){
				next;
			}
			#   die pack('c',substr($imei,$counter++,1));
			writeData(1, $ff, substr($imei,$counter++,1));
			if($counter>=length($imei)){
				$counter=0;
			}
		}
	}
	else{
		copy_file("$path/$const::DIR_TEMPLATE/$_[0].comm", $_[1]);
	}
	echo("$dest ($year) written\n");

}

sub writeData
{
	my $bintype=shift;
	my $src=shift;
	open(OUTF, ">>$dest") or mydie("$! $dest");
	binmode(OUTF);
	open(INF, "<$src") or mydie("No file $src");
	binmode(INF);
	my @data=<INF>;
	my $body=join('', @data);
	close(INF);
	my $imeichar=shift;
	if(length($body)>8){
		print OUTF pack('c',$imeichar).$body;
		#	echo("$src\t$bintype\t$imeichar\n");
	}
	close(OUTF);
}

sub do_jar{
	my($suite, $prod, $outfile, $mainclass)=@_;
	open(INF, "<$path/$const::DIR_TEMPLATE/MANIFEST.MF") or mydie($!);
	my @data=<INF>;
	close(INF);
	my $template=join("",@data);
	$template=~s/<SUITENAME>/$suite/isg;
	$template=~s/<PRODUCT>/$prod/isg;
	$template=~s/<VERSION>/$const::VERSION/isg;
	$template=~s/<VENDOR>/$const::VENDOR/isg;
	$template=~s/<MAINCLASS>/$mainclass/isg;
	
	my $desc;
	my $rev;
	if ($mainclass eq $GeoAMclass) {
		$desc = $const::DESCR_GEO;
		$rev = get_revision(1);
	}
	else {
		$desc = $const::DESCR_CALENDAR;
		$rev = get_revision(0);
	}
	$template=~s/<DESCR>/$desc/isg;
	$template=~s/<REVISION>/$rev/isg;
	echo("Revision = $rev\n");

	open(INF, ">$path/$const::DIR_TEMPLATE/mf") or mydie("$path/$const::DIR_TEMPLATE/mf $!");
	print INF $template;
	print INF "\r\n";
	close(INF);
	my $cmd=ensure_slash(const::JAR($jar_path, $outfile, "$path/$const::DIR_TEMPLATE/mf", "$path/$const::DIR_TEMP", $winda));
	echo("Exec: $cmd\n");
	mydie("\tERROR: creating archive") if system($cmd);
	unlink("$path/$const::DIR_TEMPLATE/mf");
	my $asize= -s $outfile;
	$template.="\nMIDlet-Jar-Size: $asize\n";
	my $jad=$outfile;
	$jad=~s/r$/d/is;
	$outfile=~s/.+[\/\\]//is;
    print 'is_local='.int($islocal)."\n";
	if(!$islocal){
		require CGI;
		my $jarurl=$outfile;
		$jarurl=~s/\..+//is;
        my $pf=$PREFIX;
        if($mainclass ne $GeoAMclass){
            $pf='Amax';
        }
		$jarurl=~/(\d\d).+?(\d{4})$/is;
		$outfile="$1_$pf-$2.jar";
		my $tjad=$jad;
		$tjad=~s/d$/t/is;
        if($config=~/demo/){ # demo is received by email link
            $jarurl=$outfile;
        }
        else{
            my $serv='mobi.astromaximum.com';
            $serv.='/mobi' if $serv!~/mobi/is;
            $jarurl='http://'.$serv."/data.php?r=".$jarurl;
        }
        print "$tjad\n$jarurl\n";
		open(FFF, ">$tjad") or mydie("$jad: $!");
		print(FFF $template."MIDlet-Jar-URL: $jarurl\n");
		close(FFF);
	}
	$template.="MIDlet-Jar-URL: $outfile\n";
	open(FFF, ">$jad") or mydie("$jad: $!");
	print(FFF $template);
	close(FFF);
}

sub do_messjar{
	if(!$messjar){
		echo("Messjar disabled by user\n");
		return;
	}
	my ($jar)=@_;
	echo("Messjaring $jar...\n");

	open(INF, "<$jar") or echo("No file");
	binmode(INF);
	my @data=<INF>;
	my $body=join('', @data);
	close(INF);


	#=head
	my $backup=$jar;
	$backup=~s/\.jar/\.zip/is;
	open(OutF,">$backup") or mydie("Cannot open file");
	binmode(OutF);
	print OutF $body;
	close (OutF);

	echo("  backup: $backup\n");
	#=cut
	#$jar=~s/\.jar/\.zip/is;


	$body=mess_compression_local($body);
	if($body=~s/Amdata\.class/Amaxdata\.dat/sg){
		$body=mess_add_special_entry($body);
	}
	else{
		echo("No Amaxdata found\n");
	}

	#$body=mess_compression_central($body);
	#$body=mess_direrase($body);

	open(OutF,">$jar") or mydie("Cannot open file");
	binmode(OutF);
	print OutF $body;
	close (OutF);
	echo("Finished.\n");
}


sub mess_compression_local {
	my $body=shift;
	$body=~s/(.+?)($file_sign)/$2/is;
	my $out=$1;
	my $count=0;
	while($body=~s/($file_sign.+?)($file_sign)/$2/is){
		my $sect=$1;
		my $seed=pack('c',int(rand(6)));
		if($sect!~/(META\-INF|Amaxdata|icon\.png)/s){
			$sect=~s/($file_sign.{4})./$1$seed/is;
			++$count;
		}
		$out.=$sect;
	}
	$out.=$body;
	echo("  mess_compression_local - $count times\n");
	return $out;
}

sub mess_add_special_entry {
	my $body=shift;
	echo("  add_special_entry\n");
	$body=~/(.+?Amaxdata\.dat)(.+?)($file_sign.+)/is;
	my($before, $inn, $after)=($1,$2,$3);
	#   mydie($after);
	$after=~s/($fdir_sign.+)//is;
	$body=$1;
	my $inn_sz=length($inn);
	$inn.=$after;
	#   mydie($body);
	my $start=0;
	my @apos; my @acrc;
	my $old=0;
	my $ind=index($after,$file_sign,$start);
	do{
		push(@apos,$ind-$old);
		push(@acrc,unpack('L',substr($after, $ind+0xe, 4)));
		$start=$ind+1;
		#	echo("$ind\n");
		$old=$ind;
		$ind=index($after,$file_sign,$start);
	}while($ind>=0 and $#apos<10); # only first 10 files recorded
	$ind=0;
	substr($inn,0,1)=pack('c',$#apos+1);

	while($#apos>=0){
		my $p=shift(@apos);
		echo("$p, ");
		substr($inn,$ind*6+1,6)=pack('nN',$p, shift(@acrc)^$p);
		$ind++;
	}
	$ind*=6+1;
	while($ind<$inn_sz){
		substr($inn,$ind++,1)=pack('c',rand(256));
	}
	my $crc32=new Digest::Crc32();
	my $crc=pack('L',$crc32->strcrc32($inn));
	my $sz=length($inn);
	$sz=pack('LL',$sz,$sz);
	$before=~s/(.+$file_sign.{4}).(.{5}).{12}/$1\0$2$crc$sz/s;
	$body=~s/.(.{5}).{12}(.{18}Amaxdata\.dat)/\0$1$crc$sz$2/s;
	return $before.$inn.$body;
}

sub mess_compression_central {
	my $body=shift;
	$body=~s/(.+?)($fdir_sign)/$2/is;
	my $out=$1;
	my $count=0;
	while($body=~s/($fdir_sign.+?)($fdir_sign)/$2/is){
		my $sect=$1;
		my $seed=pack('c',9);
		if($sect!~/META\-INF/s){
			$sect=~s/($fdir_sign.{6})./$1$seed/is;
			++$count;
		}
		$out.=$sect;
	}
	$out.=$body;
	echo("  mess_compression_central - $count times\n");
	return $out;
}

sub mess_direrase {
	my $body=shift;
	$body=~s/(.+?)($file_sign)/$2/is;
	my $out=$1;
	my $count=0;
	while($body=~s/($file_sign.+?)($file_sign)/$2/is){
		my $sect=$1;
		if($sect=~/\A.{22}\0{4}.+\Z/s){
			++$count;
		}
		else{
			$out.=$sect;
		}
	}
	$out.=$body;
	$body=$out;
	$body=~s/(.+?)($fdir_sign)/$2/is;
	$out=$1;
	while($body=~s/($fdir_sign.+?)($fdir_sign)/$2/is){
		my $sect=$1;
		if($sect=~/\A.{24}\0{4}.+\Z/s){
			next;
		}
	}
	$out.=$body;
	echo("  mess_direrase - $count times\n");
	return $out;
}

sub unzip{
	my $uz='unzip';
	$uz="$path/$uz" if $winda;
	my $cmd=sprintf($const::UNZIP, $uz, $_[0], "$path/$const::DIR_TEMP");
	echo("Exec: $cmd\n");
	system($cmd);
}

sub do_timebomb{
	my($ofs, $delta)=@_;
	my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime();
	$hour+=$ofs;
	my @sign=(pack('N',0x01234567),pack('N',0x89abcdef));
	my $tm2=POSIX::mktime($sec, $min, $hour, $mday, $m,$y,0,0,-1)*1000;
	#		$tz_ofs=$tm-$tm2;

	echo("Installing time bomb: $ofs hours, delta = $delta min...\n\n");
	#print POSIX::strftime( "Current time is %B %d, %Y - %H:%M:%S GMT\n", $sec,$min,$hour,$mday,$m,$y,$wday );
	echo("Begin time:  ".timebomb_install($tm2,$sign[0]));

	#print POSIX::strftime( "Deadline time is  %B %d, %Y - %H:%M:%S GMT\n", $sec,$min,$hour,$mday,$m,$y,$wday );
	$tm2=POSIX::mktime($sec, $min + $delta, $hour, $mday, $m,$y,0,0,-1)*1000;
	echo("  End time:  ".timebomb_install($tm2,$sign[1]));

	echo("Finished.\n");

}


sub join_datafiles2 # year, destfile, data_listref
{
	my $year=shift;
	open(OUTF, ">$_[0]");
	my @bins=@{$_[1]};
	my @buf;
	binmode(OUTF);
	print OUTF pack('n',$year);
	print OUTF pack('n',$#bins+1);
	foreach (@bins){
		print length($_)."\n";
		print OUTF pack('n',length($_));
	}
	foreach (@bins){
		print OUTF $_;
	}
	close(OUTF);
}

sub rm_all{ #dir to erase
	my $dir=shift;
	foreach (glob("$dir/*")){
		if(-f $_){
			unlink $_;
		}
		else{
			rm_all($_);
		}
	}
	rmdir($dir);
}

sub timebomb_install # time, sign
{
	my $tm2=int($_[0]/4096);
	my @classes=glob("$path/$const::DIR_TEMP/*.class");
	foreach my $class(@classes){
		open(INF, "<$class") or mydie("No file $class");
		binmode(INF);
		my @data=<INF>;
		close(INF);
		my $body=join('', @data);
		my $hextm=pack("N",$tm2);
		#	echo($tm2,',',unpack("H*",$hextm));
		my $pos=index($body, $_[1]);
		if($pos>=0){
			substr($body, $pos, length($hextm))=$hextm;
			open(INF, ">$class") or mydie("No file $class");
			binmode(INF);
			print INF $body;
			close(INF);
			my($sec,$min,$hour,$mday,$m,$y,$wday,$yday);
			if($ARGV[2]){
				($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = gmtime($tm2*4.096);
			}
			else{
				($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime($tm2*4.096);
			}
			return POSIX::strftime( "%B %d, %Y - %H:%M:%S ", $sec,$min,$hour,$mday,$m,$y,$wday).' 0x'.unpack("H*",$hextm)."\n";
		}
	}
	mydie("timebomb_install failed!!!\n");
}

sub encode85 # in: filename, out: string
{
	die "$path/3d_party/encode85$ext not found" unless -e "$path/3d_party/encode85$ext";
    open (INF, "$path/3d_party/encode85$ext -w 0 $_[0] |") or mydie("Cannot open pipe $!");
    binmode(INF);
    my @amax_data=<INF>;
    close(INF);
    my $data = join('', @amax_data);
    open (OUTF, ">$_[0].85");
    binmode (OUTF);
    print (OUTF $data);
    close (OUTF);
    return $data;
}

sub datafile_install # magic string, datafile, split flag
{
    my $magic = $_[0];
	my $grep_ver = `grep --version`;
	die "Not GNU grep command" unless $grep_ver =~/GNU grep/;
    my $cmd = "grep -r $magic $const::DIR_TEMP";
	print "$cmd\n";
    my $class_fn = `$cmd`;

    my $class_body;
    if ($class_fn =~ /(\w+\.class)/is) {
        $class_fn = "$const::DIR_TEMP/$1";
		open(INF, "<$class_fn") or mydie("No file $class_fn");
		binmode(INF);
		my @data=<INF>;
		close(INF);
		$class_body=join('', @data);
    }
    else {
        mydie("Magic string '$magic' not found in class files");
    }

    my $class_data;
    my $data = encode85($_[1]);

    my $SPLIT_CHUNK = 31888;
    my $idx = 0;
    my $inject_count = 0;
    my $again = 1;
    do {
        my $magic_str = $_[0].$idx++;
        my $magic_content = pack("na*",length($magic_str), $magic_str);
        my @part= split(/$magic_content/s, $class_body);
        if(scalar(@part) == 2) {
            $data =~ s/(.{0,$SPLIT_CHUNK})//s;
            my $chunk = $1;
            #$again = length ($chunk) == $SPLIT_CHUNK;
            my $content=pack("na*",length($chunk), $chunk);
            $class_body = $part[0].$content.$part[1];
            echo("Injected $magic_str into $class_fn, chunk length ".length($chunk)."\n");
            ++$inject_count;
        }
        else {
            $again = 0;
        }
    }
    while ($again);
    if (!$inject_count) {
        mydie("Inject failed");
    }
    open(INF, ">$class_fn") or mydie("No file $class_fn");
    binmode(INF);
    print INF $class_body;
    close(INF);
    echo("Written $class_fn\n");
}

sub inject_lang{ # lang, isdemo
	my($lang, $demo)=@_;
	$demo=0;
	my $use_amtext=$lang=~s/\-//is; # write only EV_MSG if lang contains '-'
	my $dest="$path/$const::DIR_TEMP";
	my @bins=glob("$path/$const::DIR_INTERPRET/$lang/*.txt");
	unless(scalar(@bins)){
		echo("\nNo files for '$lang' language ($path/$const::DIR_INTERPRET/$lang/*.txt)\n\n");
		exit(1);
	}
	my @buf;
	my $body;
#	my @demo_allowed=qw(
#	EV_VOC EV_SIGN_ENTER EV_MOON_MOVE EV_ASP_EXACT_MOON EV_DEGPASS0 EV_DEGPASS1
#	EV_DEGPASS2 EV_DEGPASS3 EV_RETROGRADE EV_MSG
#	);
#
#	my %demo_events;
#	if($demo){
#		echo("Demo mode: filtering events\n");
#		foreach(@demo_allowed){
#			my $id=$tools::eventType{$_};
#			mydie("Unknown demo event <$_>, $id") unless defined($id);
#			$demo_events{$_}=$id;
#		}
#	}
#	else{
#		%demo_events=%tools::eventType;
#	}
	my %demo_events=%tools::eventType;

	echo("Cleaning $dest dir\n");
	my @clean=glob("$dest/*");
	foreach (@clean){
		unlink $_ if $_=~/[\/\\]\d+$/is;
	}
	#mydie($tools::eventType{'EV_VOC'});
	foreach my $ff(@bins){
	  next if $ff!~/\.txt$/is;
	  open(InF, "<$ff") or mydie("No file $ff");
		@buf=<InF>;
		close(InF);
		my $body="@buf";
		$outbuf=''; my $recnum=0;
		$buf[0]=~/\!\!type\s*(\w+)/i;
		my $evt=$1;

		if($tools::eventType{$evt}!~/^\d+$/){
			echo("Event $evt not defined in $ff! Skipped\n");
			next;
		}
		unless(defined($demo_events{$evt})){
			echo("skipped from demo\n");
			next;
		}
	  next if $evt ne 'EV_MSG' and $use_amtext;
	  next if $evt eq 'EV_MSG' and $demo eq 'amtext';
#	  echo("\n**** $ff: *****\n");
		$buf[1]=~/\!\!params\s*(\d+)/i;
		$paramcount=$1;
		$buf[2]=~/\!\!planet\s*(.+)/i;
		my $planet=$1;
		my $RESERVED_CHARS='*^$}>{~#@=';

		foreach my $ln(@buf){
			my $line=$ln;
			$line=~s/\/\/.+//is;
			next if $line!~/%[\d\s\,\-]+%/;
			#		next if $line=~/\A\s*\Z/is;
			#		echo("$line\n");
			$line=~s/\s*\Z//is;
			$line=~s/\.+\Z//is if $evt ne 'EV_MSG';
			$line=~s/.*?%(.*?)%\s*//is;
			write_record($1);
			#		echo($line."\n");
			if($evt ne 'EV_MSG'){
				for(my $i=0; $i<length($RESERVED_CHARS); $i++){
					my $char='\\'.substr($RESERVED_CHARS,$i,1);
					my @cnt=$line=~/([$char])/isg;
					if($#cnt>=0){
						warn "@cnt" if $char eq '$';
						if($#cnt%2 !=1){
							echo("\n  not matched - $1 in\n   $line \n");
							++$errors;
						}
						else{
							if($char eq '\@'){
								for(my $j=0; $j<length($RESERVED_CHARS)-1; $j++){
									my $ch=substr($RESERVED_CHARS,$j,1);
									if(index('#~{=',$ch)==-1){
										add_event_char($evt,'\\'.$ch);
									}
								}
							}
							else{
								add_event_char($evt,$char);
							}
						}
					}
				}
			}
			writeUTF($line);
			$recnum++;
		}
		my $len;
		#	warn $outbuf;
		#	exit();
		do{
			use bytes; $len=length($outbuf)+11;
		};
		#	mydie($flag);
#		echo("$len, $planet\n");
		$output=pack('nNcnna*',$tools::eventType{$evt},$len,$planet,$paramcount,$recnum,$outbuf);
		#	mydie($output);
		if($config ne 'lang'){
			open(OF, ">$dest/$tools::eventType{$evt}") or mydie("No file $dest/$tools::eventType{$evt}");
			binmode(OF);
			print OF $output;
			close(OF);
		}
		$output='';
		$outbuf='';

	}
	if($errors==0){
		return 0 if $demo;
        echo("\n---Add these lines into SummItem.java---\n\n");
		while (my($key, $value) = each %hash) {
			$value=~s/\\//isg;
			echo("    topics.put(new Integer(Event.$key), \"$value\");\n");
			delete $hash{$key};   # This is safe
		}
        echo("\n---cut here---\n\n");
	}
	else{
		echo("\n-------- $errors error(s) found. Compilation aborted! --------\n");
        exit(1);
	}

	#my $inp=<STDIN>;
}

sub writeUTF
{
	my $param=shift;
	#	$param = decode("cp1251", $param);
	my $len;
	do{
		use bytes; $len=length($param);
	};
	$outbuf.=pack('na*', $len, $param);
	#	$outbuf.=$param;
	#	mydie($outbuf);
}

sub write_record
{
	my $par=shift;
	my @params=split(/,/,$par);
	if($paramcount>0 && $#params+1!=$paramcount){
		echo("\n  parameters should be $paramcount in $par , not $#params\n");
		++$errors;
	}
	for(my $i=0; $i<$paramcount; $i++){
		$outbuf.=pack('n',$params[$i]);
	}
}

sub add_event_char
{
	if($hash{$_[0]}!~/$_[1]/is){
		$hash{$_[0]}.=$_[1];
	}
}

sub random # path, extension if file, undef if dir
{
	my ($fn,$id,$flag);
	do{
		$id=''; $flag=1;
		for(my $i=0; $i<12; $i++){
			$id.=int(rand(10));
		}
		$fn="$_[0]/$id$_[1]";
		if($_[1]){
			$flag=0 if -f $fn;
		}
		else{
			$flag=0 if -d $fn;
		}
	}while(!$flag);
	return ($fn, $id);
}

sub echo{
	print $_[0];
	print FLOG $_[0];
}

sub mydie{
	print $_[0];
	print FLOG "DIE: $_[0]";
	close FLOG;
	die $_[0];
}

sub get_revision{ # num
    open (REV, "<$path/rev.txt") or mydie ("Revision file error: $!");
    my @revs = <REV>;
    close (REV);
#    echo (scalar(@revs)."\n");
    my $result;
    if (scalar(@revs) == 1) {
        my @body = split (/\s/s, "@revs");
        $result = $body[$_[0]];
    } else {
        $result = $revs[$_[0]];
    }
#    echo("revision='$result'\n");
    mydie ("No revision") unless $result > 0;
	return $result;
}

# vi:et:ts=4:sw=4
