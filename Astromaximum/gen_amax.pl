#!/usr/bin/perl
use strict;
use POSIX;

our $path;
our $file_sign="\x50\x4B\x03\x04";
our $fdir_sign="\x50\x4B\x01\x02";

our $winda=$^O=~/Win/is;

our %eventType=qw(EV_VOC 0 EV_SIGN_ENTER 1 EV_ASP_EXACT 2 EV_RISE 3 EV_DEGREE_PASS 4 
	EV_VIA_COMBUSTA 5 EV_RETROGRADE 6 EV_ECLIPSE 7 EV_TITHI 8 EV_NAKSHATRA 9 EV_SET 10
	EV_DECL_EXACT 11 EV_NAVROZ 12 EV_WEEK 13 EV_PLANET_HOUR 14 EV_STATUS 15 EV_SUN_RISE 16 
	EV_MOON_RISE 17 EV_MOON_MOVE 18 EV_SEL_DEGREES 19 EV_DAY_HOURS 20 EV_NIGHT_HOURS 21 
  EV_SUN_DAY 22 EV_MOON_DAY 23 EV_GRID_DATE 24 EV_MOON_PHASE 25 EV_ZODIAC_SIGN 26 
  EV_PANEL 27 EV_FAST_BUTTON 28 EV_DEG_2ND 29 EV_WEEK_GRID 30 EV_MONTH_GRID 31 
  EV_DECUMBITURE 32 EV_DECUMB_ASPECT 33 EV_DECUMB_BEGIN 34 EV_SUN_DEGREE_LARGE 35 
  EV_MOON_SIGN_LARGE 36 EV_HELP 37 EV_ASP_EXACT_MOON 38 EV_DEGPASS0 39 EV_DEGPASS1 40
  EV_DEGPASS2 41 EV_DEGPASS3 42 EV_HELP0 43 EV_HELP1 44 EV_ASTRORISE 45 EV_ASTROSET 46
  EV_APHETICS 47 EV_FAST 48 EV_ASCAPHETICS 49 EV_MSG 50 EV_LAST 51
  );

our %eventFlags=qw(EF_PLANET1 2 EF_PLANET2 4 EF_DEGREE 8 EF_SHORT_DEGREE 64);
our $output=''; our $paramcount=0; our $outbuf; our $errors=0; 
our %hash;

$0=~/(.+)[\\\/]/is;
$path=$1;
#my $path=`pwd`;
chomp($path);
if(!$path){
    $path='.';
}
require "$path/genconst.pm";

if(!scalar(@ARGV)){
    print "This script generates ready-to-use $const::PRODUCT $const::VERSION distribution.\n";
    print "Parameters:\n";
    print "\t<config>: [rebuild|notest|release|tb|demo]\n";
    print "\t<year>\n";
    print "\t<lang>\n";
    print "\t<loclist file>\n";
    print "\t<output jar>, or '-' for default\n";
    print "\t[imei|timebomb|tb_timeout]\n";
    print "\t[nomessjar]\n";
    exit(1);
}

require "$path/tools.pm";
require "$path/Crc32.pm";

our $TEST_YEAR=2007; # default year for Demo

our $config=shift(@ARGV);
our $year=shift(@ARGV);
our $lang=shift(@ARGV);
our $loclist=shift(@ARGV);
our $outfile=shift(@ARGV);

if($config eq 'rebuild'){
    print "Rebuilding all configs...\n";
    my $antpath;
    my @app=(
      '/home/willow/nb6/java1/ant/bin/ant', 
      'd:/Program Files/nb6/java1/ant/bin/ant.bat'
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
        die "BUILD ERROR" if system($cmd);
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
our $messjar=1;

my $argv="@ARGV";
$messjar=0 if $argv=~/nomessjar/is;

die "Invalid year '$year'" if $year!~/^\d{4}$/is;
$loclist=ensure_slash($loclist);
die "Invalid loclist '$loclist'" if ! -f $loclist;
my $dest='';

if($config=~/(2006|demo)/is){
    $year=$TEST_YEAR;
    unless($outfile=~/\.jar/is){
	$outfile="$path/Astromaximum/deploy/$const::PRODUCT".'Demo.jar' ;
    }
}

$year=~/\d\d(\d\d)/is;
my $ye=$1;

if($config=~/geo-$/is){
    unless($outfile=~/\.jar/is){
	$outfile="$path/Astromaximum/deploy/Geo$ye.jar";
    }
}

unless($outfile=~/\.jar/is){
    die "Invalid filename: '$outfile'" unless $outfile eq '-';
    $outfile="$path/Astromaximum/deploy/$const::PRODUCT$ye.jar" ;
    print "Outfile is '-', setting to $outfile\n";
}
$outfile=ensure_slash($outfile);
print "Processing <$config> for $year lang=$lang using locations from $loclist...\n";

rm_all("$path/$const::DIR_TEMP");

if($config=~/tb/is){
    my $ofs=shift(@ARGV);
    $ofs=0 unless $ofs;
    my $delta=shift(@ARGV);
    $delta=30 unless $delta;
    unzip("$path/$const::DIR_TEMPLATE/Astromaximum-tb.jar");
    inject_lang($lang); 
    inject_amdata();
    do_timebomb($ofs, $delta);
    inject_common($year, "$path/$const::DIR_TEMP/common.dat");
    inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
    inject_icon("res/");
    do_jar("$const::PRODUCT$ye", $outfile);
#    do_messjar($outfile);
    exit(0);
}

if($config=~/(2006|demo)/is){
    $year=2006;
    unzip("$path/$const::DIR_TEMPLATE/AstromaximumDemo.jar");
    inject_lang($lang, 'demo'); 
    inject_amdata();
    inject_common($year, "$path/$const::DIR_TEMP/c.dat");
    inject_locations($year, $loclist, "$path/$const::DIR_TEMP/l.dat");
    inject_icon("res/");
    do_jar("AstromaximumDemo", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/notest$/is){
    unzip("$path/$const::DIR_TEMPLATE/Astromaximum-notest.jar");
    inject_lang($lang); 
    inject_common($year, "$path/$const::DIR_TEMP/common.dat");
    inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
    inject_icon("res/");
    do_jar("$const::PRODUCT$ye", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/release$/is){
    my $imei=shift(@ARGV);
    $imei='0' x 15 unless $imei;
    unzip("$path/$const::DIR_TEMPLATE/Astromaximum.jar");
    inject_lang($lang); 
    inject_common($year, "$path/$const::DIR_TEMP/common.dat", $imei);
    inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
    inject_amdata();
    inject_icon("res/");
    do_jar("$const::PRODUCT$ye", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/release_logger$/is){
    my $imei=shift(@ARGV);
    $imei='0' x 15 unless $imei;
    unzip("$path/$const::DIR_TEMPLATE/Astromaximum-logger.jar");
    inject_lang($lang); 
    die "logger";
    inject_common($year, "$path/$const::DIR_TEMP/common.dat", $imei);
    inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
    inject_amdata();
    inject_icon("res/");
    do_jar("$const::PRODUCT$ye", $outfile);
    do_messjar($outfile);
    exit(0);
}

if($config=~/geo-$/is){
    unzip("$path/$const::DIR_TEMPLATE/GeoAM.jar");
    inject_locations($year, $loclist, "$path/$const::DIR_TEMP/locations.dat");
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
    copy_file("$path/Astromaximum/src/Amdata.class", "$path/$const::DIR_TEMP/Amdata.class");
    open(INF,"<$path/Astromaximum/src/Amdata.class") or die "Cannot open file";
    binmode(INF);
    my @body=<INF>;
    close (INF);
    open(OUTF,">$path/$const::DIR_TEMP/Amdata.class") or die "Cannot open file";
    binmode(OUTF);
    print OUTF join('', @body);
    close (OutF);
}

sub inject_icon{ #subdir
    $ye=~/(\d)$/is;
    open(INF,"<$path/images/icons/$1.png") or die "Cannot open file $path/images/icons/$1.png";
    binmode(INF);
    my @body=<INF>;
    close (INF);
    open(OUTF,">$path/$const::DIR_TEMP/$_[0]"."icon.png") or die "Cannot open file $!";
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
	    	my $dfile=ensure_slash("$path/data/archive/$_[0]/$1/$2.dat");
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

    $path1.="/data/archive/$year";
	open(OUTF, ">$dest") or die "$! $dest";
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
    open(INF, "<$path/$const::DIR_TEMPLATE/MANIFEST.MF") or die $!;
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

    open(INF, ">$path/MANIFEST.MF") or die "$path/MANIFEST.MF $!";
	print INF $template;
	print INF "\r\n";
    close(INF);
    my $cmd=ensure_slash(const::JAR("$path/", $outfile, "$path/MANIFEST.MF", "$path/$const::DIR_TEMP", $winda));
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
    if(!$messjar){
        print "Messjar disabled by user\n";
        return;
    }
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
    rm_all("$path/$const::DIR_TEMP");
    copy_file($_[0], $outfile);
    my $cmd=sprintf($const::UNZIP, $_[0], "$path/$const::DIR_TEMP");
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
    my @classes=glob("$path/$const::DIR_TEMP/*.class");
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
    die "Operation failed!!!\n";
}

sub inject_lang{ # lang, isdemo
    my($lang, $demo)=@_;
    my $dest="$path/$const::DIR_TEMP";
    my @bins=glob("$path/interpret/$lang/*.txt");
    die "No files for '$lang' language\n" unless scalar(@bins);
    my @buf;
    my $body;

    my @demo_allowed=qw(
      EV_VOC EV_SIGN_ENTER EV_MOON_MOVE EV_ASP_EXACT_MOON EV_DEGPASS0 EV_DEGPASS1
      EV_DEGPASS2 EV_DEGPASS3 EV_RETROGRADE EV_MSG
    );


    my %demo_events;
    if($demo){
      print "Demo mode: filtering events\n";
      foreach(@demo_allowed){
        my $id=$eventType{$_};
        die "Unknown demo event <$_>, $id" unless defined($id);
        $demo_events{$_}=$id;
      }
    }
    else{
      %demo_events=%eventType;
    }


    print "Cleaning $dest dir\n";
            my @clean=glob("$dest/*");
            foreach (@clean){
                    unlink $_ if $_=~/[\/\\]\d+$/is;
            }
    #die $eventType{'EV_VOC'};  
    foreach my $ff(@bins){
            open(InF, "<$ff") or die "No file $ff";
            @buf=<InF>;
            close(InF);
            print "\n**** $ff: *****\n";
            my $body="@buf";
            $outbuf=''; my $recnum=0;
            $buf[0]=~/\!\!type\s*(\w+)/i;
            my $evt=$1;

            if($eventType{$evt}!~/^\d+$/){
                    print "Event $evt not defined in $ff! Skipped\n";
                    next;
            }
            unless(defined($demo_events{$evt})){
              print "skipped from demo\n";
              next;
            }
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
    #		print "$line\n";
                    $line=~s/\s*\Z//is;
                    $line=~s/\.+\Z//is if $evt ne 'EV_MSG';
                    $line=~s/.*?%(.*?)%\s*//is;
                    write_record($1);
    #		print $line."\n";
                    if($evt ne 'EV_MSG'){
                      for(my $i=0; $i<length($RESERVED_CHARS); $i++){
                              my $char='\\'.substr($RESERVED_CHARS,$i,1);
                              my @cnt=$line=~/([$char])/isg;
                              if($#cnt>=0){
                                      warn "@cnt" if $char eq '$';
                                      if($#cnt%2 !=1){
                                              print "\n  not matched - $1 in\n   $line \n";
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
    #	die $flag;	 
            print "$len, $planet\n";
            $output=pack('nNcnna*',$eventType{$evt},$len,$planet,$paramcount,$recnum,$outbuf);
    #	die $output;
            open(OF, ">$dest/$eventType{$evt}") or die "No file";
            binmode(OF);
            print OF $output;
            close(OF);
            $output='';
            $outbuf='';

    }
    if($errors==0){
      return if $demo;
      while (my($key, $value) = each %hash) {
            $value=~s/\\//isg;
            print "    topics.put(new Integer(Event.$key), \"$value\");\n";
            delete $hash{$key};   # This is safe
            }

    }
    else{
            print "\n-------- $errors error(s) found. Compilation aborted! --------\n";
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
#	die $outbuf;
}

sub write_record
{
	my $par=shift;
	my @params=split(/,/,$par);
	if($paramcount>0 && $#params+1!=$paramcount){
		print "\n  parameters should be $paramcount in $par , not $#params\n";
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
