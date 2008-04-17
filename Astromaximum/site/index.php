<?php 
$EXEC=1;
include_once('mobi/lang.php');
$main='home';
if(isset($_GET['p'])){
	$main=$_GET['p'];
}
if(strcmp($main, 'pwdrestore')==0){
	session_start();
}
else{
	sess_start();
}
include_once('mobi/dbconnect.php');
lang_load("mobi/html");
$chac=check_access();
$user_ok=($chac>=0 and $chac!=1);
$city_count=711;
$price=60;
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
<title>ASTROMAXIMUM - первый астрологический календарь для мобильных телефонов </title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<meta name="author" content="Unknown"/>
<meta name="copyright" content="Copyright (c) by ASTROMAXIMUM.de"/>
<meta name="keywords" content="ключи"/>
<meta name="description" content="описание"/>
<link href="astro.css" rel="stylesheet" type="text/css"/>
<script type="text/javascript">
	function findObj(id) {
	  return (document.all?document.all[id]:document.getElementById(id));
	}
</script>
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
<p><b>GMT <span id="mtime">&nbsp;</span></b></p>
</div>
<div id="menu">
<a href="?<?php echo $lang_ ?>">главная</a> | 
<a href="?<?php echo $lang_ ?>&amp;p=0_0">инструкция</a> | 
<?php anchor('buy') ?>купить</a> | 
<?php anchor('citylist') ?>список городов</a> |
<?php anchor('dl') ?>загрузка городов</a> | 
<a href="#">контакты</a>
<?php 
//echo "<br/>";print_r($_REQUEST);
$btn1=$i18['DEMO']."<br/>+ ".$i18['CITY_MODULE']; $btn1_link="demo";
$btn2=$i18['ORDER']." $$price<br/>+ $city_count ".$i18['_CITIES'];
if($chac==0){
	echo <<<ADMIN_TB
	<p>| 
	<a href="?$lang_&amp;p=db_stats&amp;mode=env">окружение</a> |  
	<a href="?$lang_&amp;p=db_stats&amp;mode=data">статистика</a> |  
	<a href="?$lang_&amp;p=upload">загрузка городов</a> |
	<a href="?$lang_&amp;p=usermgr">пользователи</a> |
	</p>
ADMIN_TB;
}
if($user_ok){
	$current_year=get_year();
	$btn1="ЗАГРУЗИТЬ<br/>ГОРОД"; $btn2="ЗАГРУЗИТЬ<br/>КАЛЕНДАРЬ - $current_year"; $btn1_link="dl";
}
if($chac==1){
	$btn2=$i18['ORDER']." $$price<br/>+ $city_count ".$i18['_CITIES'];
}
if($chac!=-1){
	$session_prompt=<<<SP1
	<p>Здравствуйте, <b>{$_SESSION['username']}</b> ! </p>
	<p><a href="mobi/dl/logout.php"><strong>выход</strong></a></p> 
SP1;
}
else{ 
	$session_prompt=<<<FRM
  <script type="text/javascript">
  <!--
	  function checklogin(){
	  	if(!findObj('ilog').value || !findObj('ipwd').value){
	  		return false;
	  	}
		findObj("flog").submit();
	  }
	-->
	</script>
	<form id="flog" action="?$lang_&amp;p=login&amp;to=$main" method="post"> 
	<input id="ilog" name="login"/> логин  <br /><br />
	<input id="ipwd" name="pass" type="password"/> пароль <br /><br />
	<a id="aenter" href="javascript:void(0)" onclick="checklogin(); return false"><strong>вход</strong></a> | 
	<a href="?$lang_&amp;p=pwdrestore"><strong>восстановить регистрацию</strong></a>
	</form> 
FRM;
} 
?> 
</div>

<?php if(!$user_ok || strcmp($main, 'dl')){ ?>
<div id="demo"><?php anchor($btn1_link); echo $btn1 ?></a></div> 
<div id="buy"><?php anchor('buy'); echo $btn2 ?></a></div>
<?php } ?>

<div id="leftColumn"> 
<script type="text/javascript">
<!--	
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
-->  
</script>
<?php	echo $session_prompt ?>

<?php if($show_topics){ ?>
<h5>темы календаря </h5>
<p><?php anchor("0_1")?><img src="i/ico.gif" alt="" /> <br /><b>деловая активность, подписание контрактов</b></a></p>
<p><?php anchor("0_2")?><img src="i/ico.gif" alt="" /> <br /><b>торговля, финансы</b></a></p>
<p><?php anchor("0_3")?><img src="i/ico.gif" alt="" /> <br /><b>регистрация предприятия, получение лицензии, открытие магазина</b></a></p>
<p><?php anchor("0_4")?><img src="i/ico.gif" alt="" /> <br /><b>устройство на работу, найм</b></a></p>
<p><?php anchor("0_5")?><img src="i/ico.gif" alt="" /> <br /><b>недвижимость, строительство, сельское хозяйство</b></a></p>
<p><?php anchor("0_6")?><img src="i/ico.gif" alt="" /> <br /><b>поездки, учеба</b></a></p>
<p><?php anchor("0_7")?><img src="i/ico.gif" alt="" /> <br /><b>любовь, брак</b></a></p>
<p><?php anchor("0_8")?><img src="i/ico.gif" alt="" /> <br /><b>медицина, косметология</b></a></p>
<p><?php anchor("0_9")?><img src="i/ico.gif" alt="" /> <br /><b>ход болезни (декумбитура)</b></a></p>
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
				if($user_ok){
					include($fn);
					if(strcmp($main, '0_0')){
						echo "<p><br/><a href=\"?$lang_&amp;p=0_".$main{0}."\"><strong>назад к теме</strong></a></p>";
					}
				}
				else{
					reg_warning("Просмотр документации");
				}
			}
			else{
				include($fn);
			}
		}
		else{
			echo "<h3>Запрашиваемой страницы не существует</h3>";
		}
	} 
?>
</div><!-- end content div -->
<div id="bottom"><p>Copyright &copy; 2007 S&amp;W Axis. All rights reserved.   &nbsp;&nbsp;    <a href="http://goglus.com">design goglus</a></p></div>
</body>
</html>
