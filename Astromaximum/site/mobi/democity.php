<?php
$EXEC=4;
include_once("lang.php");
sess_start();
$sess=session_name().'='.session_id();
include_once("amtools.php");
include_once('dbconnect.php');
$chac=check_access();
if($chac==-1){
	redirect("/");
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum.mobi</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>
<div id="hdr" class="hdr">Astromaximum</div>
<div id="cont">
<div class="hr"></div>
<?php
	foreach($DEMO_CITY as $i=>$city){
		echo '<br/><a href="geo.php?lvl=10&amp;p0='.$i."&amp;$sess\"/> ".$city;
	}
?>
</div>
</body>
</html>
