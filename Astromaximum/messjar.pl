#!/usr/bin/perl
use strict;

use Digest::Crc32;
#die;
our $file_sign="\x50\x4B\x03\x04";
our $fdir_sign="\x50\x4B\x01\x02";

$0=~/(.+\\)/is;
my $jar=$ARGV[0];
#$jar='D:\Willow\prj\astrology\nomad_prj\Astromaximum\dist\midp2y2007\Astromaximum.jar';
#$jar='d:\projects\nomad_prj\Astromaximum\dist\midp2y2007\Astromaximum.zip';
print "Messjaring $jar...\n";

undef $/ ;
open(InF, "<$jar") or print "No file";
binmode(InF);
my $body=<InF>;
close(InF);


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

$body=~s/Amdata\.class/Amaxdata\.dat/sg;

$body=mess_compression_local($body);
$body=mess_add_special_entry($body);

#$body=mess_compression_central($body);
#$body=mess_direrase($body);

open(OutF,">$jar") or die "Cannot open file";
binmode(OutF);
print OutF $body;
close (OutF);
print "Finished.\n";

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
	$body=~/(.+?Amaxdata\.dat)(.+?)($file_sign.+)/is;
	my($before, $inn, $after)=($1,$2,$3);
#	die $after;
	$after=~s/($fdir_sign.+)//is;
	$body=$1;
	my $inn_sz=length($inn);
	$inn.=$after;
#	die $body;
	my $start=0;
	my @apos; my @acrc;
	my $old=0;
	my $ind=index($after,$file_sign,$start);
	do{
		push(@apos,$ind-$old);
		push(@acrc,unpack('L',substr($after, $ind+0xe, 4)));
		$start=$ind+1;
#		print "$ind\n";
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