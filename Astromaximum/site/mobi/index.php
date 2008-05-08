<?php 
$EXEC=2;
include_once('lang.php');
include_once('dbconnect.php');
include_once('amtools.php');
$main='home';
if(isset($_GET['p'])){
	$main=$_GET['p'];
}
sess_start();
$sess=session_name().'='.session_id();
lang_load("html");
$chac=check_access();
$user_ok=($chac>=0 and $chac!=1);
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
		if(isset($_GET['to'])){
			$main=$_GET['to'];
		}
	}
	else{
		include_once("ipblock.php");
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
if(!preg_match("/^[_\d]+$/is", $main)){
	$main="m_$main";
}
$dir=dirname($_SERVER['SCRIPT_FILENAME']);
$fn="$dir/html/$lang/$main.php";
if(!file_exists($fn)){
	$fn="$dir/html/$main.php";
	if(!file_exists($fn)){
		$main='home';
		$fn="$dir/html/$lang/m_home.php";
	}
}
if(preg_match("/^(demo)$/is", $main)){
//	$show_topics=0;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum.mobi</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>
<div id="hdr" class="hdr">
<?php 
	if(strcmp($main,"m_home")==0){
		echo "Astromaximum";
	}
	else{
		echo "<a href=\"?$lang_&amp;p=selector&amp;$sess\">Astromaximum</a>";
	}
?>
</div>
<div id="cont">
<p>
<?php
	if($custom_content){
		echo $custom_content;
	}
	else{
		if(file_exists($fn)){
			$manual_requested=preg_match("/^[_\d]+$/is", $main);
			if($manual_requested){
				if($user_ok){
					echo str_replace('src="mobi/', 'src="', file_get_contents($fn));
					if(strcmp($main, '0_0')){
						echo "<p><br/><a href=\"?$lang_&amp;p=0_".$main{0}."\"><strong>{$i18['BACK_TOPIC']}</strong></a></p>";
					}
				}
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
</p>
</div>
<div id="ftr">
<?php
if($chac!=-1){
	echo "<div class=\"hr\"></div>";
	echo "user: ".$_SESSION['username'];
	echo " &nbsp; <a href=\"dl/logout.php\">logout</a>";
}
else{
	echo '<br/>* for demo:<br/> log: '.$GLOBALS['amax']['demo_login'].
		'<br/> pas: '.$GLOBALS['amax']['demo_pass'];
}
?>
</div>
</body></html>
