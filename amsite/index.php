<?php
include_once('amtools.php');
if(isset($_GET['mobi']) || is_mobile()>0 || (strcmp($_SERVER['SERVER_NAME'],"localhost")==0)){
   echo "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
} 
else {
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
Your USER_AGENT is: <?php echo $_SERVER['HTTP_USER_AGENT'] ?><br/><br/>
You better should visit <a href="http://astromaximum.de/">http://astromaximum.de/</a>
</body></html>
<?php
   exit;
}
$current_year=get_year();   
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
<!-- <?php echo $_SERVER['HTTP_USER_AGENT'] ?><br/>-->
<a href="year.php">Download a city</a><br/>
<a href="html/ru/0_0.xhtml">Astro help</a><br/>
<a href="about.xhtml">About Astromaximum</a><hr/>
<form action="dl/prg.php" method="post">
<optgroup>	
<input type="radio" name="lang" value="en">en</select>
<input type="radio" name="lang" value="en" checked>ru</select>
</optgroup><hr/>
<optgroup>	
<input type="radio" name="dest" value="PH" checked>Phone</select>
<input type="radio" name="dest" value="PC">PC</select>
</optgroup><br/>
<?php echo $current_year-1 ?> <input type="submit" name="mode" value="demo" style="width:6em"><br/>
<?php echo $current_year ?> <input type="submit" name="mode" value="trial" style="width:6em">
</form>
<hr/>
</div>
<div id="ftr"></div></body></html>
