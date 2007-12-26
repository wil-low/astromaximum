<?php
	include_once('../dbconnect.php');
	include_once('../amtools.php');
	$default_city_ids='319,307,273,186';  #London #New York #Kiev #Moscow    

	if(!isset($_GET['mode'])) exit;
	$year=get_year();
	$isdemo=0;
	if(strcmp($_GET['mode'], 'demo')==0){
		$year--;
		$isdemo=1;
	}
	else if(strcmp($_GET['mode'], 'trial')!=0){
		exit;
	}
	if(isset($_GET['lang'])){
	}
	else{
		$lang='EN';
	}
	$outp=array();
	global $DIR_FILES, $DIR_SOURCE;
	$dfil="../$DIR_FILES";
	$dsrc="../$DIR_SOURCE";
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="$dsrc/$fn";
#	echo "$dsrc/$destfile";
	$cmd="perl gen_amax.cgi demo $year $lang $srcdir $dsrc/$fn.jar nomessjar";
	exec($cmd, $outp);
#	echo "<p>$cmd</p>";
#	echo implode('<br>',$outp);
	$fn=create_jar($year, $default_city_ids, "$dsrc/$fn.jar", $isdemo, 
		'AstromaximumDemo', "source/$year.comm");
#	unlink("$dsrc/$fn.jar");
#	unlink("$dsrc/$fn.jad");
	header("Location: ../data.php?t=$fn");
#	exit;
		
?>