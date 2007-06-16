#!/usr/bin/perl
use strict;
use warnings;
use Digest::Crc32;
use Math::BigInt lib => 'GMP';

my @str=split(/\n/,<<END);
"com.sonyericsson.IMEI",
"com.samsung.IMEI",
"com.samsung.imei",
"com.samsungmobile.IMEI",
"com.samsungmobile.imei",
"com.siemens.mp.imei",
"phone.imei",
"phone.IMEI",
"com.nokia.mid.imei",
"com.nokia.IMEI",
"device.imei",
"device.IMEI",
"imei",
"IMEI",
"microedition.hostname"
END

$0=~/(.+\\)/is;
our $path=$1;

my $chunk='cODE';
my @buf;
my $crc32=new Digest::Crc32();
my $imei='';
for(my $i=0; $i<=$#str; $i++){
  $str[$i]=~/\"(.+?)\"/is;
	my $enc=str_encode($1, $i);
	$imei.=pack('N',length($enc)).$chunk.$enc.pack('N',$crc32->strcrc32($chunk.$enc));
}
open(INF, $path."images\\panel.png") or die "No file";
binmode(INF);
@buf=<INF>;
close(INF);
my $body=join('',@buf);
my ($before, $after)=$body=~/(.+?)(.{4}IDAT.+)/s;
$body=$before.$imei.$after;
open(OUTF, ">$path".'Astromaximum\\src\\res\\panel.png') or die "No file";
binmode(OUTF);
print OUTF $body;
close(OUTF);

sub str_encode {
  my ($ss, $i)=@_;
  my $out='';
  while(length($ss)>0){
  	$ss=~s/\A(.{1,8})//is;
  	$out.=str_encode2($1,$i).".";
#  	die $out;
  }
  return $out;
}

sub str_encode2 {
  my ($ss, $i)=@_;
  my $sum=Math::BigInt->new('0');
  for(my $j=0; $j<length($ss); $j++){
    $sum->blsft(8);
    $sum->badd(ord(substr($ss,$j,1)));
  }
  $ss='';
  while($sum>0){
  	my $rem=$sum%($i+15);
  	$sum=($sum-$rem)/($i+15);
  	if($rem>9){
  		$rem=chr(ord('a')+$rem-10);
  	}
  	$ss=$rem.$ss;
  }
  return $ss;
}
