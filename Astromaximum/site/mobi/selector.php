<?php
$BIG_SITE="http://astromaximum.de/";
$MOBI_SITE="http://astromaximum.mobi/";
if(isset($_GET['lang'])){
	if(isset($_GET['lang'])){
		$lang=$_GET['lang'];
	}
	else{
		$lang='en';
	}
}
//print_r($_REQUEST);
if(isset($_GET['dest']) && isset($_POST['btn'])){
	$dest=$_GET['dest'];
	$btn=$_POST['btn'];
	$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
	if(!strpos($data_php, "mobi")){
		$data_php.="/mobi";
	}
	if(strcmp($dest, 'pc')==0){ // PC links
		$desturl=$BIG_SITE."?lang=$lang";
		switch($btn){
			case 1:	$desturl.="&p=dl"; break;
			case 2:	unset($desturl); break;
//			case 3:	$desturl=$BIG_SITE."&p=dl"; break;
			case 4:	$desturl.="&p=demo"; break;
			case 5:	$desturl.="&p=buy"; break;
		}
	}
	else{ // Phone links
		switch($btn){
			case 1:	$desturl="year.php"; break;
//			case 2:	$desturl="html/$lang/0_0.xhtml"; break;
			case 3:	$desturl="html/$lang/about.xhtml"; break;
			case 4:	$desturl="dl/prg.php?mode=demo&lang=$lang&dest=$dest"; break;
			case 5:	$desturl="dl/prg.php?mode=trial&lang=$lang&dest=$dest"; break;
		}
	}
	if(isset($desturl)){
//		echo $desturl;
		header("Location: $desturl");
	}
	exit;
}
include_once("lang.php");
//include_once('amtools.php');
$current_year=get_year();   
lang_load("html");
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
<div class="hr"></div>
<br/>
<form action="<?php echo htmlentities($_SERVER['REQUEST_URI']) ?>" method="post"><p>
<input type="submit" accesskey="1" name="btn" value="1"/> <?php echo $i18['SEL_DCITY'] ?><br/>
<input type="submit" accesskey="2" name="btn" value="2" disabled="disabled"/> <?php echo $i18['SEL_AH'] ?><br/>
<input type="submit" accesskey="3" name="btn" value="3"/> <?php echo $i18['SEL_ABOUT'] ?><br/>
<input type="submit" accesskey="4" name="btn" value="4"/> <?php echo $current_year-1 ?> <?php echo $i18['SEL_DEMO'] ?>*<br/>
<input type="submit" accesskey="5" name="btn" value="5"/> <?php echo $current_year ?>* <?php echo $i18['SEL_CLONLY'] ?>
</p></form>
<p class="centered">* <?php echo $i18['SEL_CHK1'] ?><br/>
<?php echo "{$i18['SEL_CHK2']} ".gmdate("M j Y") ?>
</p>
</div>
<div id="ftr"></div>
</body></html>