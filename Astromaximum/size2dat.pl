#!/usr/bin/perl
use strict;
#use warnings;

my $isRegenerate = 0; # set this to 1 if you need to pretty reformat files

$0=~/(.+)[\\\/]/is;
my $path=$1;
if(!$path){
	$path='.';
}

my @bins=glob("$path".'/Astromaximum/arrays/*.txt');
die "No files $path".'/Astromaximum/arrays/*.txt' if $#bins<0;

require "$path/tools.pm";

my %revt = reverse(%tools::eventType);

foreach my $ff(@bins){
	my $InF=undef;
	our $OutF=undef;
	open($InF, "<$ff") or die "No file";
	$ff=~/.+[\\\/](\w+)\./is;
	my $fname=$1;
	open(OutF, ">$path/Astromaximum/arrays/$fname.dat") or
        die "No file $path/Astromaximum/arrays/$fname.dat";
	binmode(OutF);
	my $count=-1;
	my @buf=<$InF>;
	close($InF);

if($isRegenerate){
	open(OutF2, ">$path/Astromaximum/src/res/$fname.txt2") or
        die "No file $path/Astromaximum/src/res/$fname.txt2";

    print OutF2 "#  Astromaximum item coordinates table\n\n";
    print OutF2 sprintf("#  %2s,%2s,%2s,%2s   %4s    %5s    %-20s    %2s,%2s,%2s,%2s\n\n",
        'l', 't', 'w', 'h', 'page', 'ho,ve', 'event type', 'l', 'r', 'u', 'd');
}

	my $line;
	while($line=shift(@buf)){
        my $comment='';
#		$line=~s/\#.+//is;
		if($line=~/\{(.+)\},?\s*(.+)\s*/is){
			$line=$1;
            $comment=$2;
            $comment=~s|//(\S)|// $1|;
            $comment=~s|//|#|;
		}
		else{
			next;
		}
		my @num=split(/\s*\,\s*/, $line);
#        die join(', ', @num);
        if($num[7]=~/^EV_/){
            $num[7] = $tools::eventType{$num[7]};
#            die $num[7];
        }
        my $tp = $revt{$num[7]};
        if(!$tp){
            warn "Type not found: $num[7]\n";
            $tp = $num[7];
        }

if($isRegenerate){
        print OutF2 sprintf("{  %2d,%2d,%2d,%2d,   %3d,   %2d,%2d,   %-20s,   %2d,%2d,%2d,%2d  }   %s",
            $num[0], $num[1], $num[2], $num[3], $num[4], $num[5],
            $num[6], $tp, $num[8], $num[9], $num[10], $num[11], $comment);
}

#		warn ">$line<" if $#num!=6;
		foreach my $nn(@num){
			print (OutF pack('n',$nn));
			$count=$nn if $nn>$count;
		}
	#	print $line;
	}
	++$count;
	if($fname=~/tabStop/is){
		for(my $i=0; $i<24; $i++){
			print (OutF pack('c',$count+$i));
		}
	}

if($isRegenerate){
	close(OutF2);
}
	close(OutF);
	print "$fname processed\n";
}
