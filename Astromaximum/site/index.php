<?php 
$city_count=330;
$lang="ru";
function anchor($pp){
	global $lang;
	echo "<a href=\"?lang=$lang&p=$pp\">";
} 
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>ASTROMAXIMUM - первый астрологический календарь для мобильных телефонов </title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<meta name="author" content="design by goglus.com"/>
<meta name="copyright" content="Copyright (c) by ASTROMAXIMUM.de"/>
<meta name="keywords" content="ключи"/>
<meta name="description" content="описание"/>
<link href="astro.css" rel="stylesheet" type="text/css"/>
<!-- <link rel="shortcut icon" href="/favicon.gif" /> -->
<script language="JavaScript" type="text/JavaScript">
<!--
function findObj(id) {
  return (document.all?document.all[id]:document.getElementById(id));
}
//-->
</script>
</head>
<body>
<div id="globe"><a href="index.php"><img src="i/globe.jpg" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" height="320" width="956"/></a></div>
<div id="logoText">астрологический календарь для мобильных телефонов</div>
<div id="lang"> <a href="#">DE</a> | <a href="#">ENG</a> | <b>RU</b><br /></div>
<div id="menu"><a href="#">главная</a> |  <a href="#">инструкция</a> |  <a href="#">купить</a> | <a href="mobi/dl/city.html">модули городов</a> | <a href="#">контакты</a> </div>
<div id="demo"><a href="#">СКАЧАТЬ DEMO +<br />  <?php echo $city_count ?> модулей городов</a></div> 
<div id="buy"><a href="#">КУПИТЬ 22$ +<br />  <?php echo $city_count ?> модулей городов</a></div>

<div id="leftColumn"> 
	<h6>GMT <span id="mtime">&nbsp;</span></h6>
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
<p>
<input name="login"/> <a href="#">логин</a>  <br /><br />
<input name="pass"/> <a href="#">пароль</a> <br /><br />
<a href="#"><strong>вход</strong></a> | <a href="#"><strong>регистрация</strong></a> 
</p>

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
</div><!-- end leftColumn div -->
<div id="content">
<?php
	$page='home';
	if(isset($_GET['p'])){
		$main=$_GET['p'];
	}
	if(!preg_match("/^[\w_\d]+$/is", $main)){
		$main='home';
	}
	$dir=dirname($_SERVER['SCRIPT_FILENAME']);
	$fn="$dir/mobi/html/$lang/$main";
	if(!file_exists($fn)){
		$fn="$dir/mobi/html/$lang/home";
	}
	$content='';
	readfile($fn, $content);
	echo "$content";
?>	
</div><!-- end content div -->
<div id="bottom"><p>Copyright &copy; 2007 Astromaximum. All rights reserved.   &nbsp;&nbsp;    <a href="http://goglus.com">design goglus</a></p></div>
</body>
</html>
