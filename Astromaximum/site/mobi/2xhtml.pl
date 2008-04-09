#!/usr/bin/perl
use strict;
#use warnings;

my @hdr;


my $head_t=<<EOHEAD;
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>%TITLE%</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=30"/>
<link rel="stylesheet" type="text/css" href="../../style.css"/>
</head>
<body>
EOHEAD

my $lang='ru';
if($lang eq 'ru'){
	@hdr=("К началу","К заголовку темы","Деловая активность, контракты","Торговля, финансы",
		"Регистрация, лицензии, магазин", "Устройство на работу", "Недвижимость, хозяйство",
		"Поездки, учеба", "Любовь, брак", "Медицина, косметология", "Ход болезни");
}
my $img_dim=12;
my $alter={
	p0=>'SO',p1=>'MO',p2=>'ME',p3=>'VE',p4=>'MA',p5=>'JU',p6=>'SA',p7=>'UR',p8=>'NE',p9=>'PL',
};

my $path;
$path=$1 if $0=~/(.+[\/\\])/is;
mkdir $path.'html/ru' or warn "$!";

my @files=glob($path.'txt/ru/*.txt');
foreach(@files){
        print ">$_\n";
	convert($_);
}

sub convert{
	my $fn=shift;
	open(INF, "<$fn" ) or die "No file $!";
	binmode(INF);
	my @data = <INF>;
	close(INF) ;
	my $body=join('',@data);
	$body=~s/^\xef\xbb\xbf//is;
	$body=~s/^\s+//is;
	$body=~s/\s+$//is;
	$fn=~s/\.txt/.php/is;
	$fn=~s/txt/html/is;
	$fn=~/.+[\/\\](.+?)\./is;
	my $fid=$1;
	my $header;
	if($fid ne '0_0'){
		$fid=~/^(\d)/is;
		my $theme=$1;
		if($theme){
			$header=' .. .. .. .. <a class="sidepad" href="0_'.$theme.'.xhtml">'.$hdr[1].'</a>';
		}
		$header='<a class="sidepad" href="0_0.xhtml">'.$hdr[0].'</a>'.$header;
	}
	my $header_main=$head_t.'<div id="hdr" class="hdr">'.$header.'</div>';
	my $tit;
	if($body=~s/<title>(.+)<\/title>//is){
		$tit="<h4>$1</h4>";
#		$header_main=~s/%TITLE%/$tit/is;
	}
	my $footer='<div id="ftr">'.$header.'</div></body></html>';
	$body=~s/<img (\w[\d_]+)\s*>/<img src="mobi\/i\/$1\.gif" alt="$alter->{$1}" width="$img_dim" height="$img_dim"\/>/isg;
	$body=~s/<a (\w[\d_]+)>/<a href="?lang=$lang&amp;p=$1">/isg;
#	$body=~s/<i>/<span class="comment">/isg;
#	$body=~s/<b>/<span class="alert">/isg;
#	$body=~s/<\/[ib]>/<\/span>/isg;
#	$body='<div id="cont">'.$body.'</div>';
#	$body="$header_main\n$body\n$footer";
	$body='<?php if(!isset($EXEC)) die("Access restricted") ?>'."\n$tit$body";
	open(OUTF, ">$fn" ) or die "$! $fn";
	binmode(OUTF);
	print(OUTF $body);
	close(OUTF);
}
