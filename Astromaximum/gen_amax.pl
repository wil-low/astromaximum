#!/usr/bin/perl
use strict;
use POSIX;

our $path='';
our $file_sign="\x50\x4B\x03\x04";
our $fdir_sign="\x50\x4B\x01\x02";

our $winda=$^O=~/Win/is;

$0=~/(.+)[\\\/]/is;
my $path=$1;
#my $path=`pwd`;
chomp($path);
$path.="/" if $path;
require $path.'genconst.pm';

if(!scalar(@ARGV)){
    print "This script generates ready-to-use $const::PRODUCT $const::VERSION distribution.\n";
    print "Parameters:\n";
    print "\t<config>: [rebuild|notest|release|tb|demo]\n";
    print "\t<year>\n";
    print "\t<loclist file>\n";
    print "\t<output jar>, or '-' for default\n";
    print "\t[imei|timebomb|tb_timeout]\n";
    exit(1);
}

require $path.'tools.pm';
require $path.'Crc32.pm';

our $config=shift(@ARGV);
our $year=shift(@ARGV);
our $loclist=shift(@ARGV);
our $outfile=shift(@ARGV);

if($config eq 'rebuild'){
    print "Rebuilding all configs...\n";
    my $antpath;
    my @app=(
      '/home/willow/nb6beta2/java1/ant/bin/ant', 
      'd:/Program Files/nb6beta2/java1/ant/bin/ant.bat'
#      '/home/willow/netbeans-5.5.1/ide7/ant/bin/ant', 
#      'd:/netbeans-5.5/ide7/ant/bin/ant.bat',
#      'd:/Program Files/netbeans-5.5.1/ide7/ant/bin/ant.bat'
    );
    foreach (@app){
      if(-f $_){
        $antpath=$_;
        last;
      }
    }
    if(!$antpath){
      die "ANT not found in this system!!!\n";
    }
    my @conf=qw(midp2y2007notest midp2y2007notest_logger midp2y2007release 
        midp2y2007release_logger midp2y2007release_tb test2006);
    foreach(@conf){
        print "\n--------------------------------\n";
        print "--- Config $_ ---\n";
        print "--------------------------------\n";
        my $cmd="\"$antpath\" -quiet -f Astromaximum/build.xml -Dconfig.active=$_ -Drebuild.only=true clean jar";
        print "$cmd\n";
        die "BUILD ERROR"if system($cmd);
    }
    my $conf="DefaultConfiguration";
    print "\n--------------------------------\n";
    print "--- Config geo: $conf ---\n";
    print "--------------------------------\n";
    my $cmd="\"$antpath\" -quiet -f GeoAM/build.xml -Drebuild.only=true clean jar";
    print "$cmd\n";
    die "BUILD ERROR"if system($cmd);
    exit(0);
}

die "Invalid year '$year'" if $year!~/^\d{4}$/is;
$loclist=ensure_slash($loclist);
die "Invalid loclist '$loclist'" if ! -f $loclist;
my $dest='';

if($config=~/(2006|demo)/is){
    $year=2006;
    unless($outfile=~/\.jar/is){
			$outfile=$path."Astromaximum/deploy/$const::PRODUCT".'Demo.jar' ;
    }
}

$year=~/\d\d(\d\d)/is;
my $ye=$1;

if($config=~/geo-$/is){
    unless($outfile=~/\.jar/is){
	$outfile=$path."Astromaximum/deploy/Geo$ye.jar";
    }
}

unless($outfile=~/\.jar/is){
    die "Invalid filename: '$outfile'" unless $outfile eq '-';
    $outfile=$path."Astromaximum/deploy/$const::PRODUCT$ye.jar" ;
    print "Outfile is '-', setting to $outfile\n";
}
$outfile=ensure_slash($outfile);
print "Processing <$config> for $year using locations from $loclist...\n";

rm_all("$path$const::DIR_TEMP");

if($config=~/tb/is){
    my $ofs=shift(@ARGV);
    $ofs=0 unless $ofs;
    my $delta=shift(@ARGV);
    $delta=30 unless $delta;
    unzip("$path$const::DIR_TEMPLATE/Astromaximum-tb.jar");
    inject_amdata();
    do_timebomb($ofs, $delta);
    inject_common($year, "$path$const::DIR_TEMP/common.dat");
    inject_locations($year, $loclist, "$path$const::DIR_TEMP/locations.dat");
    inject_icon("res/");
    do_jar("$const::PRODUCT$ye", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/(2006|demo)/is){
    $year=2006;
    unzip("$path$const::DIR_TEMPLATE/AstromaximumDemo.jar");
    inject_amdata();
    inject_common($year, "$path$const::DIR_TEMP/c.dat");
    inject_locations($year, $loclist, "$path$const::DIR_TEMP/l.dat");
    inject_icon("res/");
    do_jar("AstromaximumDemo", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/notest$/is){
    unzip("$path$const::DIR_TEMPLATE/Astromaximum-notest.jar");
    inject_common($year, "$path$const::DIR_TEMP/common.dat");
    inject_locations($year, $loclist, "$path$const::DIR_TEMP/locations.dat");
    inject_icon("res/");
    do_jar("$const::PRODUCT$ye", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/release$/is){
    my $imei=shift(@ARGV);
    $imei='0' x 15 unless $imei;
    unzip("$path$const::DIR_TEMPLATE/Astromaximum.jar");
    inject_common($year, "$path$const::DIR_TEMP/common.dat", $imei);
    inject_locations($year, $loclist, "$path$const::DIR_TEMP/locations.dat");
    inject_amdata();
    inject_icon("res/");
    do_jar("$const::PRODUCT$ye", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/geo-$/is){
    unzip("$path$const::DIR_TEMPLATE/GeoAM.jar");
    inject_locations($year, $loclist, "$path$const::DIR_TEMP/locations.dat");
    inject_icon();
    do_jar("Geo$ye", $outfile);
    do_messjar($outfile);
    exit(0);
}

die "Invalid config";

sub ensure_slash{
	$_[0]=~s/\//\\/isg if $winda;
	return $_[0];
}


sub copy_file{
    open(INF,"<$_[0]") or die "Cannot open file $_[0]: $!";
    binmode(INF);
    my @body=<INF>;
    close (INF);
    open(OUTF,">$_[1]") or die "Cannot open file $_[1]: $!";
    binmode(OUTF);
    print OUTF join('', @body);
    close (OutF);
    print "cp-> $_[1]\n";
}

sub inject_amdata{
    return;
    copy_file($path."Astromaximum/src/Amdata.class", "$path$const::DIR_TEMP/Amdata.class");
    open(INF,"<$path"."Astromaximum/src/Amdata.class") or die "Cannot open file";
    binmode(INF);
    my @body=<INF>;
    close (INF);
    open(OUTF,">$path$const::DIR_TEMP/Amdata.class") or die "Cannot open file";
    binmode(OUTF);
    print OUTF join('', @body);
    close (OutF);
}

sub inject_icon{ #subdir
    $ye=~/(\d)$/is;
    open(INF,"<$path"."images/icons/$1.png") or die "Cannot open file $path"."images/icons/$1.png";
    binmode(INF);
    my @body=<INF>;
    close (INF);
    open(OUTF,">$path$const::DIR_TEMP/$_[0]"."icon.png") or die "Cannot open file $!";
    binmode(OUTF);
    print OUTF join('', @body);
    close (OutF);
    print "$const::DIR_TEMP/res/icon.png written\n";
}

sub inject_locations{
    if(!$_[0]){
			die "Usage: inject_locations.pl <year> <city list> <dest file>\n";
    }
    my @fn;
    open(IN, "<$_[1]") or die "error $!: $_[1]\n";
    while(my $ln=<IN>){
	    if($ln=~/(\w+):(Data\d\d)/is){
	    	my $dfile=ensure_slash($path."data/archive/$_[0]/$1/$2.dat");
		    push(@fn, $dfile);
	    }
    }
    close(IN);
    my $i=scalar(@fn);
    tools::join_datafiles($_[0], $i, $_[2], \@fn);
    print "$_[2] written\n";
}

sub inject_common{
    my $imei='000000000000000';
    #our $imei='359308007701623';
    #die sprintf('%x',substr($imei,0,8));
    if(scalar(@_)==0){
	die "Usage: <year> [dest dir] [IMEI]\n";
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
	    print "Invalid IMEI=$_[2],using $imei\n";
	}
    }
    $dest=$_[1] if $_[1];
    my $header=pack('nCCCCn',$year, $month, $day, $hour, $min, $day_count);
    my $path1=$path;

    $path1.="data/archive/$year/";
	open(OUTF, ">$dest") or die "$! $dest";
	binmode(OUTF);
	print OUTF $header;
	close(OUTF);

    my @bins=glob("$path1".'*.bin');
    #my @bins=glob("$path".'retro09.bin');
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
    print "$dest ($year) written\n";

}

sub writeData
{
    my $bintype=shift;
    my $src=shift;
    open(OUTF, ">>$dest") or die "$! $dest";
    binmode(OUTF);
    open(INF, "<$src") or die "No file $src";
    binmode(INF);
    my @data=<INF>;
    my $body=join('', @data);
    close(INF);
    my $imeichar=shift;
    if(length($body)>8){
	print OUTF pack('c',$imeichar).$body; 
#	print "$src\t$bintype\t$imeichar\n";
    }
    close(OUTF);
}

sub do_jar{
    my($prod, $outfile)=@_;
    open(INF, "<$path$const::DIR_TEMPLATE/MANIFEST.MF") or die $!;
    my @data=<INF>;
    close(INF);
    my $mainclass=$const::PRODUCT;
    $mainclass='GeoInstaller' if $prod=~/geo/is;
    my $template=join("",@data);
    $template=~s/<PRODUCT>/$prod/isg;
    $template=~s/<VERSION>/$const::VERSION/isg;
    $template=~s/<VENDOR>/$const::VENDOR/isg;
    $template=~s/<MAINCLASS>/$mainclass/isg;
#    $template=~s/<CODE>/$code/isg;
#	$jad=~s/<DESC>/$desc/isg;
#    $template=~s/<JAR>/$fname\.jar/isg;

    open(INF, ">$path/MANIFEST.MF") or die $!;
	print INF $template;
	print INF "\r\n";
    close(INF);
    my $cmd=ensure_slash(const::JAR($path, $outfile, "$path/MANIFEST.MF", "$path$const::DIR_TEMP", $winda));
    print "Exec: $cmd\n";
    die "\tERROR: creating archive" if system($cmd);
    my $asize= -s $outfile;
    $template.="\nMIDlet-Jar-Size: $asize\n";
    my $jad=$outfile;
    $jad=~s/jar/jad/is;
    $outfile=~s/.+[\/\\]//is;
    $template.="MIDlet-Jar-URL: $outfile\n";
    open(FFF, ">$jad") or die "$jad: $!";
    print(FFF $template);
    print(FFF "\n");
    close(FFF);
}

sub do_messjar{
    my ($jar)=@_;
    print "Messjaring $jar...\n";

    open(INF, "<$jar") or print "No file";
    binmode(INF);
    my @data=<INF>;
    my $body=join('', @data);
    close(INF);


    #=head
    my $backup=$jar;
    $backup=~s/\.jar/\.zip/is;
    open(OutF,">$backup") or die "Cannot open file";
    binmode(OutF);
    print OutF $body;
    close (OutF);

    print "  backup: $backup\n";
    #=cut
    #$jar=~s/\.jar/\.zip/is;


    $body=mess_compression_local($body);
    if($body=~s/Amdata\.class/Amaxdata\.dat/sg){
	$body=mess_add_special_entry($body);
    }
    else{
	print "No Amaxdata found\n";
    }

    #$body=mess_compression_central($body);
    #$body=mess_direrase($body);

    open(OutF,">$jar") or die "Cannot open file";
    binmode(OutF);
    print OutF $body;
    close (OutF);
    print "Finished.\n";
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
    print "  mess_compression_local - $count times\n";	
    return $out;
}	

sub mess_add_special_entry {
    my $body=shift;
    print "  add_special_entry\n";	
    $body=~/(.+?Amaxdata\.dat)(.+?)($file_sign.+)/is;
    my($before, $inn, $after)=($1,$2,$3);
#   die $after;
    $after=~s/($fdir_sign.+)//is;
    $body=$1;
    my $inn_sz=length($inn);
    $inn.=$after;
#   die $body;
    my $start=0;
    my @apos; my @acrc;
    my $old=0;
    my $ind=index($after,$file_sign,$start);
    do{
			push(@apos,$ind-$old);
			push(@acrc,unpack('L',substr($after, $ind+0xe, 4)));
			$start=$ind+1;
		#	print "$ind\n";
			$old=$ind;
			$ind=index($after,$file_sign,$start);
    }while($ind>=0 and $#apos<10); # only first 10 files recorded
    $ind=0;
    substr($inn,0,1)=pack('c',$#apos+1);

    while($#apos>=0){
	my $p=shift(@apos);
	print "$p, ";
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
    print "  mess_compression_central - $count times\n";	
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
    print "  mess_direrase - $count times\n";	
    return $out;
}

sub unzip{
    rm_all("$path$const::DIR_TEMP");
    copy_file($_[0], $outfile);
    my $cmd=sprintf($const::UNZIP, $_[0], "$path$const::DIR_TEMP");
    print "Exec: $cmd\n";
    system($cmd);
}

sub do_timebomb{
    my($ofs, $delta)=@_;
    my ($sec,$min,$hour,$mday,$m,$y,$wday,$yday) = localtime();
    $hour+=$ofs;
    my @sign=(pack('N',0x01234567),pack('N',0x89abcdef));
    my $tm2=POSIX::mktime($sec, $min, $hour, $mday, $m,$y,0,0,-1)*1000;
    #		$tz_ofs=$tm-$tm2;

    print "Installing time bomb: $ofs hours, delta = $delta min...\n\n";
    #print POSIX::strftime( "Current time is %B %d, %Y - %H:%M:%S GMT\n", $sec,$min,$hour,$mday,$m,$y,$wday );
    print "Begin time:  ", timebomb_install($tm2,$sign[0]);

    #print POSIX::strftime( "Deadline time is  %B %d, %Y - %H:%M:%S GMT\n", $sec,$min,$hour,$mday,$m,$y,$wday );
    $tm2=POSIX::mktime($sec, $min+$delta, $hour, $mday, $m,$y,0,0,-1)*1000;
    print "  End time:  ", timebomb_install($tm2,$sign[1]);

    print "Finished.\n";

}


sub join_datafiles2 # destfile, data_listref
{
    open(OUTF, ">$_[0]");
    my @bins=@{$_[1]};
    my @buf;
    binmode(OUTF);
    print OUTF pack('n',$#bins+1);
    my $i=0;
    foreach (@bins){
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
    my @classes=glob("$path$const::DIR_TEMP/*.class");
    foreach my $class(@classes){
	open(INF, "<$class") or die "No file $class";
	binmode(INF);
	my @data=<INF>;
	close(INF);
	my $body=join('', @data);
	my $hextm=pack("N",$tm2);
    #	print $tm2,',',unpack("H*",$hextm);
	my $pos=index($body, $_[1]);
	if($pos>=0){
	    substr($body, $pos, length($hextm))=$hextm;
	    open(INF, ">$class") or die "No file $class";
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
    return "Operation failed!!!\n";
}
