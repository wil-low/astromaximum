use strict;
#use warnings;
use Unicode::String;
use Encode;
my ($year, $month, $day, $hour, $min, $day_count)=(2005,12,31,0,0,367);
$0=~/(.+\\)/is;

my $path=$1;
my $InF=undef;
my @bins=glob("$path".'interpret\\*.txt');
my @buf;
my $body;



our %eventType=qw(EV_VOC 0 EV_SIGN_ENTER 1 EV_ASP_EXACT 2 EV_RISE 3 EV_DEGREE_PASS 4 
	EV_VIA_COMBUSTA 5 EV_RETROGRADE 6 EV_ECLIPSE 7 EV_TITHI 8 EV_NAKSHATRA 9 EV_SET 10
	EV_DECL_EXACT 11 EV_NAVROZ 12 EV_WEEK 13 EV_PLANET_HOUR 14 EV_STATUS 15 EV_SUN_RISE 16 
	EV_MOON_RISE 17 EV_MOON_MOVE 18 EV_SEL_DEGREES 19 EV_DAY_HOURS 20 EV_NIGHT_HOURS 21 
  EV_SUN_DAY 22 EV_MOON_DAY 23 EV_GRID_DATE 24 EV_MOON_PHASE 25 EV_ZODIAC_SIGN 26 
  EV_PANEL 27 EV_FAST_BUTTON 28 EV_DEG_2ND 29 EV_WEEK_GRID 30 EV_MONTH_GRID 31 
  EV_DECUMBITURE 32 EV_DECUMB_ASPECT 33 EV_DECUMB_BEGIN 34 EV_SUN_DEGREE_LARGE 35 
  EV_MOON_SIGN_LARGE 36 EV_HELP 37 EV_ASP_EXACT_MOON 38 EV_DEGPASS0 39 EV_DEGPASS1 40
  EV_DEGPASS2 41 EV_DEGPASS3 42 EV_HELP0 43 EV_HELP1 44 EV_LAST 45
  );
	
our %eventFlags=qw(EF_PLANET1 2 EF_PLANET2 4 EF_DEGREE 8 EF_SHORT_DEGREE 64);

my %hash;

	my @clean=glob($path."Astromaximum\\src\\*.txt");
	foreach (@clean){
		unlink $_ if $_=~/\\\d+\.txt$/is;
	}
  our $output=''; our $paramcount=0; our $outbuf; our $errors=0; 
#die $eventType{'EV_VOC'};  
foreach my $ff(@bins){
	open($InF, "<$ff") or die "No file";
	@buf=<$InF>;
	close($InF);
	print "\n\n**** $ff: *****\n";
	my $body="@buf";
	$outbuf=''; my $recnum=0;
	$buf[0]=~/\!\!type\s*(\w+)/i;
	my $evt=$1;
	
	if($eventType{$evt}!~/^\d+$/){
		print "Event $evt not defined in $ff! Skipped\n";
		next;
	}
	$buf[1]=~/\!\!params\s*(\d+)/i;
	$paramcount=$1;
	$buf[2]=~/\!\!planet\s*(.+)/i;
	my $planet=$1;

=head
	my $i=0;
	foreach my $ln(@buf){
		$ln=~s/\/\/.+//isg;
		if($ln=~s/\A(\s*\d+)/$1 %$i%/is){
			$i++; 
		}
	}
	die "@buf";
=cut

my $RESERVED_CHARS='*^$}>{~#@=';

	foreach my $ln(@buf){
		my $line=$ln;
		$line=~s/\/\/.+//is;
		next if $line!~/%[\d\s\,\-]+%/;
#		next if $line=~/\A\s*\Z/is;
#		print "$line\n";
		$line=~s/\s*\Z//is;
		$line=~s/\.+\Z//is;
		$line=~s/.*?%(.*?)%\s*//is;
		write_record($1);
#		print $line."\n";
		
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
	open(OF, ">$path"."Astromaximum\\src\\$eventType{$evt}.txt") or die "No file";
	binmode(OF);
	print OF $output;
	close(OF);
	$output='';
	$outbuf='';
	
}
if($errors==0){
#	our $OutF=undef;
#	open($OutF, ">$path".'..\Astromaximum\src\interp.txt') or die "No file";
#	binmode($OutF);
#	print $OutF $output;
#	close($OutF);
#	print "\nFile saved!\n";
  while (my($key, $value) = each %hash) {
  	$value=~s/\\//isg;
  	print "    topics.put(new Integer(Event.$key), \"$value\");\n";
  	delete $hash{$key};   # This is safe
	}

}
else{
	print "\n-------- $errors error(s) found. Compilation aborted! --------\n";
}

my $inp=<STDIN>;

sub writeUTF
{
	my $param=shift;
	$param = decode("cp1251", $param);
	my $len=Unicode::String->new($param);
	$outbuf.=pack('na*', length($len), $param);
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