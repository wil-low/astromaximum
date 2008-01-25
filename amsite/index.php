<?php
if(isset($_POST['lang'])){
	if(isset($_POST['lang'])){
		$lang=$_POST['lang'];
	}
	else{
		$lang='en';
	}
}
if(isset($_POST['dest']) && isset($_POST['btn'])){
	print_r($_POST);
	$dest=$_POST['dest'];
	$btn=$_POST['btn'];
	$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
	if(!strpos($data_php, "mobi")){
		$data_php.="/mobi";
	}
	if(strcmp($dest, 'pc')==0){ // PC links
		switch($btn){
			case 1:	$desturl="dl?lang=$lang"; break;
			case 2:	$desturl="html/$lang/0_0.xhtml"; break;
			case 3:	$desturl="html/$lang/about.xhtml"; break;
			case 4:	$desturl="dl/prg.php?mode=demo&lang=$lang&dest=$dest"; break;
			case 5:	$desturl="dl/prg.php?mode=trial&lang=$lang&dest=$dest"; break;
		}
	}
	else{ // Phone links
		switch($btn){
			case 1:	$desturl="year.php"; break;
			case 2:	$desturl="html/$lang/0_0.xhtml"; break;
			case 3:	$desturl="html/$lang/about.xhtml"; break;
			case 4:	$desturl="dl/prg.php?mode=demo&lang=$lang&dest=$dest"; break;
			case 5:	$desturl="dl/prg.php?mode=trial&lang=$lang&dest=$dest"; break;
		}
	}
	header("Location: $desturl");
	exit;
}
include_once('amtools.php');
$current_year=get_year();   
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum.mobi</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=30"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>
<div id="hdr" class="hdr">Astromaximum</div>
<div id="cont">
<!-- Opera/9.10 (Windows NT 5.1; U; ru)<br/>-->
<form action="<?php echo $_SERVER['SCRIPT_NAME'] ?>" method="post">
<p>Device:<br/>
<input type="radio" name="dest" value="ph" checked="checked"/>Phone
<input type="radio" name="dest" value="pc"/>PC
<br/>
Language:<br/>
<input type="radio" name="lang" value="en"/>en
<input type="radio" name="lang" value="ru" checked="checked"/>ru
</p>
<div class="hr"></div>
<p>
<input type="submit" accesskey="1" name="btn" value="1"/> Download a city<br/>
<input type="submit" accesskey="2" name="btn" value="2"/> Astro help<br/>
<input type="submit" accesskey="3" name="btn" value="3"/> About calendar<br/>
<input type="submit" accesskey="4" name="btn" value="4"/> <?php echo $current_year-1 ?> Demo*<br/>
<input type="submit" accesskey="5" name="btn" value="5"/> <?php echo $current_year ?>* (clients only)
</p>
</form>
<p class="centered">LONDON: <?php echo gmdate("M j Y G:i") ?>
<br/>* set your phone's timezone to London (GMT) and check out the time
</p>
</div>
<div id="ftr"></div>
</body></html>
