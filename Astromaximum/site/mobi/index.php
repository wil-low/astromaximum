<?php
include_once("lang.php");
sess_start();
include_once("dbconnect.php");
$chac=check_access();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum.mobi</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>
<div id="hdr" class="hdr"></div>
<div id="cont">
<?php
if(isset($_GET['p'])){
	$page=$_GET['p'];
	if(!preg_match("/^[_\d]+$/is", $page)){
		unset($page);
	}
}
if(isset($page) && $chac!=-1 && $chac!=1){
	$dir=dirname($_SERVER['SCRIPT_FILENAME']);
	$fn="$dir/html/$lang/$page.php";
	if(file_exists($fn)){
		$content=file_get_contents($fn);
		echo str_replace("src=\"mobi/", "src=\"", $content);
		if(strcmp($page, '0_0')){
			echo "<p><br/><a href=\"?$lang_&amp;p=0_".$page{0}."\"><strong>назад к теме</strong></a></p>";
		}
		exit;
	}
	else{
		redirect("selector.php?lang=$lang");
	}
}
?>
<form action="selector.php" method="post">
<p>
<input type="radio" name="lang" value="en"/>en
<input type="radio" name="lang" value="ru" checked="checked"/>ru
<br/>
<input type="radio" name="dest" value="ph" checked="checked"/>Mobile
<input type="radio" name="dest" value="pc"/>PC</p>
<?php if($chac==-1){ ?>
<p>
login <input name="login" type="text" size="9" maxlength="9"/><br/>
pass <input name="pass" type="password" size="9" maxlength="9" class="numinput"/><br/>
</p>
<?php } ?>
<input type="submit" accesskey="1" name="action" value="Proceed"/>
</form>
<?php if($chac==-1){ ?>
* for demo:<br/> log: 123456789<br/> pas: 012345678
<?php } ?>
</div>
<div id="ftr">
<?php
if($chac!=-1){
	echo "<div class=\"hr\"></div>";
	echo "user: ".$_SESSION['username'];
	echo " &nbsp; <a href=\"dl/logout.php\">logout</a>";
}
?>
</div>
</body></html>
