<?php 
$EXEC=1;

$META_KEYWORDS='';
$META_DESCR='';
$META_TITLE='';
function output_callback($buffer)
{
	global $META_TITLE, $META_KEYWORDS, $META_DESCR;
	// fill meta tags
	if($META_TITLE){ 
		$META_TITLE.=" - ";
	}
	$META_TITLE.="ASTROMAXIMUM"; 
	$buffer=str_replace("[[title]]", $META_TITLE, $buffer);
	$buffer=str_replace("[[keywords]]", $META_KEYWORDS, $buffer);
	$buffer=str_replace("[[description]]", $META_DESCR, $buffer);
	return $buffer;
}

ob_start("output_callback");

include_once('mobi/amtools.php');
$main='home';
if(isset($_GET['p'])){
	$main=$_GET['p'];
}
sess_start();
include_once('mobi/config.php');
include_once('mobi/lang.php');
include_once('mobi/dbconnect.php');
lang_load("mobi/html");
$chac=check_access();
$user_ok=($chac>=0 and $chac!=1);
$show_topics=1;
$custom_content='';

if(strcmp($main, 'login')==0){
	$login=''; $pass='';
	if(isset($_POST['login'])){
		$login=$_POST['login'];
	}
	if(isset($_POST['pass'])){
		$pass=$_POST['pass'];
	}
	if(login($login, $pass)){
		$main='home';
		if(isset($_GET['to']) && strcmp($_GET['to'],'demo')==0){
			$main=$_GET['to'];
		}
	}
	else{
		include_once("mobi/ipblock.php");
		$custom_content=allow_ip('login', false);
		$main='home';
	}
	if(!$custom_content){
		redirect("?$lang_&amp;p=$main");
	}	
}
if(!preg_match("/^[\w_\d]+$/is", $main)){
	$main='home';
}
$dir=dirname($_SERVER['SCRIPT_FILENAME']);
$fn="$dir/mobi/html/$lang/$main.php";
if(!file_exists($fn)){
	$fn="$dir/mobi/html/$main.php";
	if(!file_exists($fn)){
		$main='home';
		$fn="$dir/mobi/html/$lang/home.php";
	}
}
if(preg_match("/^(demo)$/is", $main)){
//	$show_topics=0;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>[[title]]</title>
<meta name="author" content="Willow"/>
<meta name="generator" content="Bluefish 1.0.7"/>
<meta name="copyright" content="Copyright (c) by S&amp;W Axis"/>
<meta name="keywords" content="[[keywords]]"/>
<meta name="description" content="[[description]]"/>
<link href="astro.css" rel="stylesheet" type="text/css"/>
<script src="./func.js" type="text/javascript"></script>
</head>
<body>
<a id="top"></a>
<div id="globe">
<img src="i/globe.jpg" width="956" height="320" usemap="#Map" alt="ASTROMAXIMUM"/>
<map id="Map" name="Map">
	<area shape="circle" coords="178,132,95" href="?<?php echo $lang_ ?>" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" />
	<area shape="rect" coords="443,87,859,165" href="?<?php echo $lang_ ?>" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" />
</map>
</div>
<div id="logoText"><?php echo $i18['AMAX_LOGO'] ?></div>
<div id="lang">
<?php
	$lng=array('DE', 'EN', 'RU');
	for($i=0; $i<count($lng); $i++){
		if($i) echo " | ";
		$lng2=strtolower($lng[$i]);
		if(strcmp($lang, $lng2)==0){
			echo "<b>$lng[$i]</b>";
		}
		else{
			echo "<a href=\"?lang=$lng2&amp;p=$main\">$lng[$i]</a>";
		}
	}
?>
<br />
<p><b>GMT <?php echo gmstrftime("%H:%M") ?></b></p>
</div>
<div id="menu">
<a href="?<?php echo "$lang_\">".$i18['MNU_HOME']?></a> | 
<?php echo anchor('manual').$i18['MNU_MAN']?></a> | 
<?php echo anchor('scr').$i18['MNU_SCRSHOTS'] ?></a> | 
<?php echo anchor('buy').$i18['MNU_BUY'] ?></a> | 
<?php echo anchor('citylist').$i18['MNU_CITYLIST'] ?></a> |
<?php echo anchor('dl').$i18['MNU_DLCIT'] ?></a> | 
<a href="#"><?php echo $i18['MNU_CONTACTS']?></a>
<?php 
//echo "<br/>";print_r($_REQUEST);
$btn1=$i18['DEMO']."<br/>+ ".$i18['CITY_MODULE']; $btn1_link="demo";
$btn2=$i18['ORDER']." {$GLOBALS['amax']['price']}<br/>+ {$GLOBALS['amax']['city_count']} ".$i18['_CITIES'];
if($chac==0){
	echo <<<ADMIN_TB
	<p>| 
	<a href="?$lang_&amp;p=env">окружение</a> |  
	<a href="?$lang_&amp;p=db_stats">статистика</a> |  
	<a href="?$lang_&amp;p=upload">загрузка городов</a> |
	<a href="?$lang_&amp;p=usermgr">пользователи</a> |
	</p>
ADMIN_TB;
}
if($user_ok){
	if(strcmp($main, 'demo')){
		$btn1=$i18['CITY_BUTTON']; $btn1_link="dl";
	}
	$btn2=$i18['TRIAL'];
}
if($chac==1){
	$btn2=$i18['ORDER']." {$GLOBALS['amax']['price']}<br/>+ {$GLOBALS['amax']['city_count']} ".$i18['_CITIES'];
}
if($chac!=-1){
	$session_prompt=<<<SP1
<p>{$i18['WELCOME']}, <b>{$_SESSION['username']}</b> ! </p>
<p><a href="mobi/dl/logout.php"><strong>{$i18['LOGOUT']}</strong></a></p> 
SP1;
}
else{ 
	$session_prompt=<<<FRM
<form id="flog" action="?$lang_&amp;p=login&amp;to=$main" method="post"> 
<input id="ilog" class="fixedinput" name="login"/> {$i18['LOGIN']} <br /><br />
<input id="ipwd" class="fixedinput" name="pass" type="password"/> {$i18['PWD']} <br /><br />
<input type="submit" class="loginbutton" onclick="return checklogin()" value="{$i18['LOG_IN']}" /> | 
<a class="loginbutton" href="?$lang_&amp;p=pwdrestore">{$i18['LOST_PWD']}</a>
</form> 
FRM;
} 
?> 
</div>
<?php 
if(!$user_ok || strcmp($main, 'dl')){ 
	echo disable_big_button('demo', $btn1, 'demo', $btn1_link);
	echo disable_big_button('buy', $btn2, 'buy', 'buy');
}
?>
<div id="leftColumn">
<?php	
echo $session_prompt 
/*
<script type="text/javascript">
  function clock() {
    now=new Date();
		var london=now.toGMTString();
		var pos=london.lastIndexOf(":");
		var tm=london.substring((pos-5),pos);
    Hello =  tm;
    e=findObj('mtime');
    if (!e) return false;
    e.innerHTML = Hello;
    Timer=setTimeout("clock()",15000);
  }
  clock();
</script>
*/  
?>

<?php if($show_topics){ ?>
<h5>темы календаря </h5>
<p><?php echo anchor("0_1")?><img src="i/ico.gif" alt="" /> <br /><b>деловая активность, подписание контрактов</b></a></p>
<p><?php echo anchor("0_2")?><img src="i/ico.gif" alt="" /> <br /><b>торговля, финансы</b></a></p>
<p><?php echo anchor("0_3")?><img src="i/ico.gif" alt="" /> <br /><b>регистрация предприятия, получение лицензии, открытие магазина</b></a></p>
<p><?php echo anchor("0_4")?><img src="i/ico.gif" alt="" /> <br /><b>устройство на работу, найм</b></a></p>
<p><?php echo anchor("0_5")?><img src="i/ico.gif" alt="" /> <br /><b>недвижимость, строительство, сельское хозяйство</b></a></p>
<p><?php echo anchor("0_6")?><img src="i/ico.gif" alt="" /> <br /><b>поездки, учеба</b></a></p>
<p><?php echo anchor("0_7")?><img src="i/ico.gif" alt="" /> <br /><b>любовь, брак</b></a></p>
<p><?php echo anchor("0_8")?><img src="i/ico.gif" alt="" /> <br /><b>медицина, косметология</b></a></p>
<p><?php echo anchor("0_9")?><img src="i/ico.gif" alt="" /> <br /><b>ход болезни (декумбитура)</b></a></p>
<?php } ?>
</div><!-- end leftColumn div -->
<div id="content">
<?php
	if($custom_content){
		echo $custom_content;
	}
	else{
		if(file_exists($fn)){
			$manual_requested=preg_match("/^[_\d]+$/is", $main);
			if($manual_requested){
#				if($user_ok){ # manuals are browsed for free
					include($fn);
					if(strcmp($main, '0_0')){
						echo "<p><br/><a href=\"?$lang_&amp;p=0_".$main{0}."\"><strong>{$i18['BACK_TOPIC']}</strong></a></p>";
					}
#				}
#				else{
#					reg_warning("Просмотр документации");
#				}
			}
			else{
				include($fn);
			}
		}
		else{
			echo "<h3>{$i18['PAGE_NOT_FOUND']}</h3>";
		}
	} 
?>

</div><!-- end content div -->
<div id="bottom"><p>Copyright &copy; 2007 S&amp;W Axis. All rights reserved.   &nbsp;&nbsp;    <a href="http://goglus.com">design goglus</a></p></div>
</body>
</html>

<?php
ob_end_flush();

function disable_big_button($id, $label, $check_page, $link_page){
	global $main, $lang_;
	$style='';
	if(strcmp($main, $check_page)){
		$label="<a href=\"?$lang_&amp;p=$link_page\">".$label.'</a>';
	}
	else{
		$style=' style="color:rgb(133,195,224)"';
	}
	return "<div id=\"$id\"$style>$label</div>\n";
}
?>
