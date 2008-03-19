<?php
include_once("lang.php");
sess_start();
$BIG_SITE="http://astromaximum.de/";
$MOBI_SITE="http://astromaximum.mobi/";
include_once("dbconnect.php");
include_once("amtools.php");
global $DEMO_CITY;
$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
if(!strpos($data_php, "mobi")){
	$data_php.="/mobi";
}
$chac=check_access();
if($chac==-1){
	$login=''; $pass='';
	if(isset($_POST['login'])){
		$login=$_POST['login'];
	}
	if(isset($_POST['pass'])){
		$pass=$_POST['pass'];
	}
	if(!login($login, $pass)){
		redirect($MOBI_SITE);
		exit;
	}
}
$chac=check_access();
$validuser=false;
if($chac>=0 && $chac!=1){
	$validuser=true;
}
$current_year=get_year();   
if(isset($_POST['btn'])){
	$dest='ph';
	$btn=$_POST['btn'];
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
			case 1:	$desturl="year.php?"; break;
			case 2:	$desturl="index.php?lang=$lang&p=0_0"; break;
			case 3:	$desturl="html/$lang/about.xhtml?"; break;
			case 4:	$desturl="dl/prg.php?mode=demo&lang=$lang&dest=$dest"; break;
			case 5:	$desturl="geo.php?lvl=10"; break;
			case 6:	$desturl="dl/prg.php?mode=trial&lang=$lang&dest=$dest"; break;
		}
	}
	if(isset($desturl)){
		$desturl.='&'.session_name().'='.session_id();
		header("Location: http://$data_php/$desturl");
	}
	exit;
}
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
<form action="<?php echo 'selector.php?'.session_name().'='.session_id() ?>" method="post"><p>
<?php echo knopka(1, $validuser).$i18['SEL_DCITY'] ?><br/>
<?php echo knopka(2, $validuser).$i18['SEL_AH'] ?><br/>
<?php echo knopka(3, true).$i18['SEL_ABOUT'] ?></p>
<p>
<?php echo knopka(4, true).$i18['SEL_DEMO'] ?> <?php echo $current_year-1 ?>*<br/>
<?php echo knopka(5, true).$i18['SEL_DEMOCITY'] ?>
</p>
<p>
<?php echo knopka(6, $validuser)."$current_year*" ?>
</p></form>
<p class="centered">* <?php echo $i18['SEL_CHK1'] ?><br/>
<?php echo "{$i18['SEL_CHK2']} ".gmdate("M j Y") ?>
</p>
<div class="hr"></div>
</div>
<div id="ftr">
<?php echo "user: ".$_SESSION['username'] ?>
&nbsp; <a href="dl/logout.php">logout</a>
</div>
</body></html>

<?php
function knopka($key, $is_valid){
  $disa="";
  if(!$is_valid){
    $key="&nbsp;";
    $disa=" disabled=\"disabled\"";
  }
  return "<input type=\"submit\" accesskey=\"$key\" name=\"btn\" value=\"$key\" style=\"width:2em;\"$disa/> ";
}
?>