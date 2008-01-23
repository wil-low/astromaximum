<?php
	include_once('../dbconnect.php');
	include_once('../amtools.php');
	include_once('nav.php');
#	print_r($_POST);
#	exit;
	$cities=array('Kiev', 'London', 'New York', 'Moscow');
	$sth=mysql_query("SELECT id FROM cities WHERE name in ('".implode("','", $cities)."')");
	$default_city_ids='';
	while($row=mysql_fetch_row($sth)){
		$default_city_ids.="$row[0],";
	}
	mysql_free_result($sth);
	$timeout_mins=180;

	if(!isset($_POST['mode'])) exit;
	$year=get_year();
	$isdemo=0;
	if(strcmp($_POST['mode'], 'demo')==0){
		$year--;
		$isdemo=1;
	}
	else if(strcmp($_POST['mode'], 'trial')!=0){
		exit;
	}
	if(isset($_POST['lang'])){
		$lang=strtoupper($_POST['lang']);
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
	$ret=0;
	exec($cmd, $outp, $ret);
//	$ret=1;
	if($ret){				
		echo $cmd;
		echo implode('<br>',$outp);
		exit;
	}
	$data_php=dirname(dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']));
	if(!strpos($data_php, "mobi")){
		$data_php.="/mobi";
	}
// show all links	
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum download</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=0"/>
<link rel="stylesheet" type="text/css" href="html/style.css"/>
</head>
<body>
<div id="hdr" class="hdr"></div>
<div id="cont">
<?php
	$url='../data.php?r='.$fn;
	if(strcmp($_POST['dest'], 'PC')==0){
		echo "<h4>Download to PC:</h4>";
		echo "<a href=\"$url\">JAR</a>&nbsp;&nbsp;";
		$url=str_replace("?r", "?d", $url);
		echo "<a href=\"$url\">JAD</a><br/><br/>";
	}
	if(strcmp($_POST['dest'], 'PH')==0){
		$url=str_replace("?r", "?t", $url);
		echo "<h4>Download to phone:</h4>";
		echo "<a href=\"$url\">JAD</a><br/><br/>";
	}
//	echo "<br><font color='red'>{$i18['VALID_LINKS']}</font><br/><br/>";
	echo '<a href="../">Back</a>';
?>
</div>
<div id="ftr"></div></body></html>

<?php		
function ask_login(){
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum.mobi</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=30"/>
<link rel="stylesheet" type="text/css" href="html/style.css"/>
<script>
	function curtime(){
		gmt1=document.getElementById("gmt").value;
		gmt2=new Date().getTime()/1000;
		var newtext=document.createTextNode(gmt2-gmt1);
		document.getElementById("gmt").appendChild(newtext);
	}
</script>
</head>
<body onLoad="curtime()">
<div id="hdr" class="hdr"></div>
<div id="cont">
<span id="ofs"></span>
<input type="hidden" id="gmt" value="<?php echo time() /*gmdate("T: Y-m-j G:i")*/ ?>">

<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">
Username: <input name="user" type="text" size="15" maxlength="15" style=' -wap-input-format: "*N"'><br/>
Password: <input name="passwd" type="password" size="15" maxlength="15" style=' -wap-input-format: "*N"'><br/>
<input type="submit" id="Submit">
</form>
</div>
<div id="ftr"></div></body></html>
	
<?php
	exit;
}
?>