<?php
	include_once('../dbconnect.php');
	include_once('../amtools.php');
	include_once('nav.php');
	$default_city_ids='273,319,307';  #London #Kiev #New York
	$timeout_mins=180;

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
	$dsrc="../$DIR_FILES";
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="$dsrc/$fn";
#	echo "$dsrc/$destfile";
	if(!$isdemo){
		if(isset($_POST['user']) && isset($_POST['passwd'])){
			if(!login($_POST['user'],$_POST['passwd'])){
				ask_login();
			}		
		}
		else{
				ask_login();
		}
	}	
	if($isdemo){
		$cmd="./gen_amax.cgi demo $year $lang $default_city_ids $dsrc/$fn.r nomessjar";
	}
	else{
		$cmd="./gen_amax.cgi tb $year ".$_POST['lang']." $default_city_ids $dsrc/$fn.r 0 $timeout_mins nomessjar";
	}
				
	exec($cmd, $outp);
	#echo $cmd;
	#echo implode('<br>',$outp);
#	$fn=create_jar($year, $default_city_ids, "$dsrc/$fn.jar", $isdemo,
#		'AstromaximumDemo', "Astromaximum", "source/$year.comm");
#	unlink("$dsrc/$fn.jar");
#	unlink("$dsrc/$fn.jad");
	$data_php=dirname(dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']));
	header("Location: http://$data_php/data.php?d=$fn");
#	exit;
		
function ask_login(){
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum.mobi</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=30"/>
<link rel="stylesheet" type="text/css" href="html/style.css"/>
</head>
<body>
<div id="hdr" class="hdr"></div>
<div id="cont">
<?php echo gmdate("D M j G:i:s T Y") ?>
<hr/>
<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">
<input type="radio" name="lang" value="EN" checked>EN</select>
<input type="radio" name="lang" value="RU">RU</select><br/>
Username: <input name="user" type="text" size="15" maxlength="15"><br/>
Password: <input name="passwd" type="password" size="15" maxlength="15"><br/>
<input type="submit" id="Submit">
</form>
</div>
<div id="ftr"></div></body></html>
	
<?php
	exit;
}
?>