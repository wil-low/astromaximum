#!/usr/bin/perl
use strict;
#use warnings;

my $isRegenerate = 1; # set this to 1 if you need to pretty reformat files
my $summary = 'Astromaximum/src/Summary.java';

$0=~/(.+)[\\\/]/is;
my $path=$1;
if(!$path){
	$path='.';
}

my @bins=glob("$path".'/Astromaximum/arrays/*.txt');
die "No files $path".'/Astromaximum/arrays/*.txt' if $#bins<0;

my (%pages, %letters, %page_names);
open (InF, "$path/$summary") or die "$!: $path/$summary";
while (my $line = <InF>) {
    if ($line =~ /(PAGE_\w+)\s*=\s*(\d+);\s*\/\/\s*size letter (\w)/) {
        my ($name, $num, $letter) = ($1, $2, $3);
        die "\$pages{$num} already defined ($letter)" if defined ($pages{$num});
        die "\$letters{$letter} already defined ($num)" if defined ($letters{$letter});
        $pages{$num} = $letter;
        $letters{$letter} = $num;
        $page_names{$letter} = $name;
        #print "\$pages{$num} = $letter\n";
        #print "\$letters{$letter} = $num\n";
    }
}

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
        open(OutF2, ">$path/Astromaximum/arrays/$fname.txt2") or
            die "No file $path/Astromaximum/arrays/$fname.txt2";

        print OutF2 "#  Astromaximum item coordinates table\n\n";
        foreach my $letter (sort (keys (%page_names))) {
            print OutF2 "#    $letter: $page_names{$letter}\n";
        }
        print OutF2 sprintf("\n#  %2s,%2s,%2s,%2s   %4s    %5s    %-20s    %2s,%2s,%2s,%2s\n\n",
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
        my $number = $num[4];
        my $old_num = $num[4];
        my $len = length($number);
        my $accum = 0;
        for (my $i = 0; $i < $len; ++$i) {
            my $letter = substr ($number, $i, 1);
            die "Unknown letter '$letter' in $number $i, $len" unless defined ($letters{$letter});
            $accum += (1 << $letters{$letter});
        }
        $num[4] = $accum;
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
            print OutF2 sprintf("{  %2d,%2d,%2d,%2d,   %10s,   %2d,%2d,   %-20s,   %2d,%2d,%2d,%2d  }   %s",
                $num[0], $num[1], $num[2], $num[3], $old_num, $num[5],
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
