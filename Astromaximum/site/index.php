<?php 
include_once('mobi/lang.php');
include_once('mobi/dbconnect.php');
$city_count=330;
$price=60;
$main='home';
$show_topics=1;
if(isset($_GET['p'])){
	$main=$_GET['p'];
}

if(strcmp($main, 'login')==0){
	include_once('mobi/dbconnect.php');
	$login=''; $pass='';
	if(isset($_POST['login'])){
		$login=$_POST['login'];
	}
	if(isset($_POST['pass'])){
		$pass=$_POST['pass'];
	}
	if(login($login, $pass)){
		if(check_access()==1){
			$main='demo';
		}
		else{
			$main='home';
		}
	}
	else{
		sleep(5);
		$main='home';
	}
	redirect("?$lang_&p=$main");	
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
	$show_topics=0;
}
function anchor($pp){
	global $lang;
	echo "<a href=\"?lang=$lang&p=$pp\">";
} 
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
﻿<title>ASTROMAXIMUM - первый астрологический календарь для мобильных телефонов </title>
<meta name="author" content="Andrei Ivushkin"/>
<meta name="copyright" content="Copyright (c) by ASTROMAXIMUM.de"/>
<meta name="keywords" content="ключи"/>
<meta name="description" content="описание"/>
<link href="astro.css" rel="stylesheet" type="text/css"/>
<!-- <link rel="shortcut icon" href="/favicon.gif" /> -->
</head>
<body>
<div id="globe"><a href="?<?php echo $lang_ ?>"><img src="i/globe.jpg" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" height="320" width="956"/></a></div>
<div id="logoText">астрологический календарь для мобильных телефонов</div>
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
			echo "<a href=\"?lang=$lng2&p=$main\">$lng[$i]</a>";
		}
	}
?>
<br /></div>
<div id="menu">
<a href="?<?php echo $lang_ ?>">главная</a> |  <a href="?<?php echo $lang_ ?>&p=0_0">инструкция</a> |  <a href="#">купить</a> | <a href="?<?php echo $lang_ ?>&p=dl">модули городов</a> | <a href="#">контакты</a>
<?php 
if(check_access()==0){
	echo "<p><a href=\"?$lang_&p=db_stats&mode=env\">окружение</a> | "; 
	echo "<a href=\"?$lang_&p=db_stats&mode=data\">статистика</a> | "; 
	echo "<a href=\"?$lang_&p=upload\">загрузка городов</a></p>";
}
?> 
</div>

<div id="demo"><?php anchor('demo') ?>СКАЧАТЬ ДЕМО<br />+ <?php echo $city_count ?> модулей городов</a></div> 
<div id="buy"><a href="#">КУПИТЬ $<?php echo $price ?><br />+ <?php echo $city_count ?> модулей городов</a></div>

<div id="leftColumn"> 
	<h6>GMT <span id="mtime">&nbsp;</span></h6>
	<script type="text/javascript">
		function findObj(id) {
		  return (document.all?document.all[id]:document.getElementById(id));
		}
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
	  function checklogin(){
	  	if(!findObj('ilog').value && !findObj('ipwd').value){
	  		return false;
	  	}
	  	findObj('aenter').disabled=1;
	  	findObj('flog').submit();
	  }
	</script>
<p>
<?php if(isset($_SESSION['username'])){
	echo "Здравствуйте, <b>{$_SESSION['username']}</b>! </p><p>";
	echo "<a href=\"mobi/dl/logout.php\"><strong>выход</strong></a>"; 
?>
<?php }else{ 
?>
	<div id="loginframe"></div>
	<form id="flog" action="<?php echo "?lang=$lang&p=login" ?>" method="post"> 
	<input id="ilog" name="login"/> <a href="#">логин</a>  <br /><br />
	<input id="ipwd" name="pass"/> <a href="#">пароль</a> <br /><br />
	<a id="aenter" href="javascript:void(0)" onclick="javascript:checklogin()"><strong>вход</strong></a> | <a href="#"><strong>восстановить пароль</strong></a>
	</form> 
<?php } 
?>
</p>

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
<p>
<?php
	if(file_exists($fn)){
		include($fn);
		if(preg_match("/^[_\d]+$/is", $main) and strcmp($main, '0_0')){
			echo "<p><br/><a href=\"?$lang_&p=0_".$main{0}."\"><strong>назад к теме</strong></a></p>";
		}
	}
	else{
		echo "<h3>Страницы не существует: $fn</h3>";
	} 
?>
</p>
</div><!-- end content div -->
<div id="bottom"><p>Copyright &copy; 2007 Astromaximum. All rights reserved.   &nbsp;&nbsp;    <a href="http://goglus.com">design goglus</a></p></div>
</body>
</html>
