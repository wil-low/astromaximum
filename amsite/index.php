<?php
include_once('amtools.php');

if($mobile_browser>0 || (strcmp($_SERVER['SERVER_NAME'],"localhost")==0)){
   echo "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
} 
else {
   header('Location: http://astromaximum.de/');
   exit;
}
   

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
<div id="cont"><hr/>
<a href="year.php">Download a city</a><br/>
<a href="html/ru/0_0.xhtml">Astro help</a><br/>
<a href="about.xhtml">About Astromaximum</a><hr/></div>
<div id="ftr"></div></body></html>
