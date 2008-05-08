<?php
	include_once('../lang.php');
	sess_start();
	include_once('../dbconnect.php');
	include_once('../amtools.php');
	lang_load("../html");
	$perl=find_perl();
	$default_city_ids=get_default_cities($GLOBALS['amax']['def_cities']);

	if(!isset($_REQUEST['mode'])) exit;
	$year=$GLOBALS['amax']['year'];
	$isdemo=0;
	if(strcmp($_REQUEST['mode'], 'demo')==0){
		$year--;
		$isdemo=1;
	}
	else if(strcmp($_REQUEST['mode'], 'trial')!=0){
		exit;
	}
	$timeout_offset=-24;
	$timeout_mins=2880;  
	
	$outp=array();
	global $DIR_FILES, $DIR_SOURCE;
	$dsrc="../$DIR_FILES";
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="$dsrc/$fn";
#	echo "$dsrc/$destfile";
	$type="d $year $lang";
	if($isdemo){
		$cmd="$perl ./gen_amax.cgi demo $year $lang $default_city_ids $dsrc/$fn.r $timeout_offset $timeout_mins nomessjar";
	}
	else{
		$cmd="$perl ./gen_amax.cgi tb $year $lang \"$default_city_ids\" $dsrc/$fn.r $timeout_offset $timeout_mins nomessjar";
		$type="t $year $lang";
	}
	$ret=0;
	exec($cmd, $outp, $ret);
//	$ret=1;
	if($ret){				
		echo $cmd;
		echo implode('<br>',$outp);
		exit;
	}
	if(!add_file($fn, $type)){
		echo mysql_error();
		exit;
	}

	$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
	if(!strpos($data_php, "mobi")){
		$data_php.="/mobi";
	}
	if(strcmp($_REQUEST['dest'], 'ph')==0){
		//echo "http://$data_php/../data.php?t=$fn";
		header("Location: http://$data_php/../data.php?t=$fn");
		exit;
	}
// show all links	
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum download</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=0"/>
<link rel="stylesheet" type="text/css" href="../style.css"/>
</head>
<body>
<div id="hdr" class="hdr"></div>
<div id="cont">
<?php
	$url='../data.php?r='.$fn;
	if(strcmp($_REQUEST['dest'], 'pc')==0){
		echo "<h4>{$i18['PC_DL']}:</h4>";
		echo "<a href=\"$url\">JAR</a>&nbsp;&nbsp;";
		$url=str_replace("?r", "?d", $url);
		echo "<a href=\"$url\">JAD</a>";
	}
	echo "<br/><br/><font color='red'>{$i18['VALID_LINKS']}</font><br/><br/>";
	echo '<a href="..">'.$i18['BACK']."</a>";
?>
</div>
<div id="ftr"></div></body></html>
