<?php
if(isset($_POST['lang'])){
	if(isset($_POST['lang'])){
		$lang=$_POST['lang'];
	}
	else{
		$lang='en';
	}
}
if(isset($_POST['dest'])){
	$dest=$_POST['dest'];
	$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
	if(!strpos($data_php, "mobi")){
		$data_php.="/mobi";
	}
	if(strcmp($dest, 'pc')==0){ // PC links
		if(isset($_POST['m_dl_x'])){
			$desturl="dl?lang=$lang";
		}
		if(isset($_POST['m_he_x'])){
			$desturl="html/$lang/0_0.xhtml";
		}
		if(isset($_POST['m_ab_x'])){
			$desturl="html/$lang/about.xhtml";
		}
		if(isset($_POST['m_demo_x'])){
			$desturl="dl/prg.php?mode=demo&lang=$lang&dest=$dest";
		}
		if(isset($_POST['m_trial_x'])){
			$desturl="dl/prg.php?mode=trial&lang=$lang&dest=$dest";
		}
	}
	else{ // Phone links
		if(isset($_POST['m_dl_x'])){
			$desturl='geo.php';
		}
		if(isset($_POST['m_he_x'])){
			$desturl="html/$lang/0_0.xhtml";
		}
		if(isset($_POST['m_ab_x'])){
			$desturl="html/$lang/about.xhtml";
		}
		if(isset($_POST['m_demo_x'])){
			$desturl="dl/prg.php?mode=demo&lang=$lang&dest=$dest";
		}
		if(isset($_POST['m_trial_x'])){
			$desturl="dl/prg.php?mode=trial&lang=$lang&dest=$dest";
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
<link rel="stylesheet" type="text/css" href="html/style.css"/>
</head>
<body>
<div id="hdr" class="hdr"></div>
<p align="center"><b>Astromaximum</b></p>
<div id="cont"><hr/>
<!-- <?php echo $_SERVER['HTTP_USER_AGENT'] ?><br/>-->
<form action="<?php echo $_SERVER['SCRIPT_NAME'] ?>" method="post">
<p>Device:
<optgroup>
<input type="radio" name="dest" value="ph" checked>Phone
<input type="radio" name="dest" value="pc">PC
</optgroup></p>
Language:
<optgroup>	
<input type="radio" name="lang" value="en">en</select>
<input type="radio" name="lang" value="ru" checked>ru</select>
</optgroup><hr/>
<optgroup>	
(1) <input type="image" src="img/ar0.png" alt="&gt;" accesskey="1" name="m_dl">Download a city<br/>
(2) <input type="image" src="img/ar0.png" alt="&gt;" accesskey="2" name="m_he">Astro help<br/>
(3) <input type="image" src="img/ar0.png" alt="&gt;" accesskey="3" name="m_ab">About calendar<br/>
(4) <input type="image" src="img/ar0.png" alt="&gt;" accesskey="4" name="m_demo"><?php echo $current_year-1 ?> Demo*<br/>
(5) <input type="image" src="img/ar0.png" alt="&gt;" accesskey="5" name="m_trial"><?php echo $current_year ?>* (clients only)
</optgroup>
</form>
<p align="center"><?php echo 'LONDON: '.gmdate("M j Y G:i") ?></p>
<br/>* set your phone's timezone to London (GMT) and check out the time
<hr/>
</div>
<div id="ftr"></div></body></html>
