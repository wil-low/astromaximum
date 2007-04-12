package tools;

use strict;

our $rar='d:\Program Files\WinRAR\winrar.exe';

sub join_datafiles # size, destfile, fname_listref
{
	my $size=$_[0];
	open(OUTF, ">$_[1]") or die "No file";
	my @bins=@{$_[2]};
	my @buf;
	my @bodies;
	binmode(OUTF);
	print OUTF pack('n',$#bins+1);
	my $i=0;
	foreach my $ff(@bins){
		open(INF, "<$ff") or die "No file";
		binmode(INF);
		undef $/ ;
		@buf=<INF>;
		close(INF);
		$bodies[$i]="@buf";
		print OUTF pack('n',length($bodies[$i]));
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
		print "$src $imeichar\n";
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

sub create_geo { # code, region, descript, destdir, locationpath, is_numbered, templatedataref
	my ($code, $reg, $desc, $destdir, $locpath, $is_numbered, $template, )=@_;
	if(!$template){
		$template=read_template();
	}
	my $fname="GeoAM-$code";
	my $jad=$$template;
	if($is_numbered){
		my $locsz= -s '.temp\\locations.dat';
		$locsz=~/(\d{0,4})$/is;
		$code=$1;
		warn $locsz;
	}
	$jad=~s/<REGION>/$reg/isg;
	$jad=~s/<CODE>/$code/isg;
	$jad=~s/<DESC>/$desc/isg;
	$jad=~s/<JAR>/$fname\.jar/isg;
	
#	die $jad;
	mkdir ".temp\\META-INF\\" unless -d ".temp\\META-INF\\";
	open(INF, ">.temp\\META-INF\\MANIFEST.MF") or die "No file";
		print INF $jad;
	close(INF);
	
	open(INF, "<GeoAM\\dist\\GeoAM.jar") or die "No file GeoAM\\dist\\GeoAM.jar";
	binmode(INF);
	my @data=<INF>;
	my $buf=join("",@data);
	close(INF);
	
	open(INF, ">$destdir$fname.jar") or die "No file $destdir$fname.jar";
	binmode(INF);
		print INF $buf;
	close(INF);
	
	my $cmd="\"$rar\" a -r -ep1 $destdir$fname\.jar .temp\\*";
	print "$cmd\n";
	system($cmd);
	
	my $asize= -s "$destdir$fname\.jar";
	$jad.="MIDlet-Jar-Size: $asize\n";
	
	open(INF, ">$destdir$fname\.jad") or die "No file";
		print INF $jad;
	close(INF);
	return $code;
}


1;
