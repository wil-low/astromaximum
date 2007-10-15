<?php
include_once('lang.php');
unset($LOGIN_MSG);

function emit_admin(){
	echo <<<ADMIN
	 <a href='geo.php'>Geo</a>
	 <b>Admin:</b> <a href='db_stats.php'>DB stats</a> 
	 <a href='upload.php'>Upload</a>
	 <br><br> 
ADMIN;
}

function microtime_float()
{
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

function emit_nav1(){
	global $lang_, $i18, $LOGIN_MSG, $START_TIME;
	$START_TIME=microtime_float();
echo <<<NAV1
<table border="1" width="100%" height=100% class=smalltxt>
	<tr height=20%>
		<td width="10%" valign=top>
			<img src="img/logo.png" border="0" alt="Astromaximum logo">
			<div align=center>
			<br>
			<form action=index.php name=lng>
			<select name="lang" onchange="javascript:document.forms.namedItem('lng').submit()">
			<option value="en">English</option>
			<option value="de">Deutsch</option>
			<option value="ru">Русский</option>
			</select>
			</form></div>
NAV1;
// entrance for clients
	if(isset($_POST['user'])){
		if(login($_POST['user'],$_POST['passwd'])){
		}
		else{
			$LOGIN_MSG='INVALID_LOGIN';
		}
		unset($_POST['user']);
	}
echo <<<NAV2
	<center>
<!--	<p><a href=geo.php?{$lang_}>{$i18['DB']}</a></p> -->
NAV2;
if($_SESSION['username']!='nobody')
{
echo <<<NAV3
	<p align=center>{$i18['WELCOME']}, {$_SESSION['username']}!</p>
	<p align=right><a href=logout.php?{$lang_}>{$i18['LOGOUT']}</a></p>
NAV3;
}
else{
echo <<<NAV4
<!--	<h4>{$i18['MEM_LOGIN']}</h4> -->
	<span class=login>
	<form method='post' action=geo.php?{$lang_} name=login>
		<table width=20%>
		<tr>
			<td>{$i18['USERNAME']}</td>
			<td><input type="text" name="user"></input></td>
			<td rowspan="2">
			<center><input type=submit value="{$i18['DB']}"></input>
			</center></td>
		</tr>
		<tr>
			<td>{$i18['PWD']}</td>
			<td><input type="password" name="passwd"></input></td>
		</tr>
	</table>
	</form></span>
	<center>
NAV4;
}

if(isset($LOGIN_MSG)){
	echo $i18[$LOGIN_MSG];
	unset($LOGIN_MSG);
}
// entrance for clients end
	echo	"<td rowspan=2 valign=top>";
}

function emit_nav2(){
	global $lang_, $i18, $LOGIN_MSG, $START_TIME;

$exec_time=microtime_float()-$START_TIME;

echo <<<NAV5
	&nbsp;</center></font></td>
	</center>
	</td></tr>
	<tr align=center valign=top>
		<td>
			<p><br><a href=test.php?{$lang_}>{$i18['TEST']}</a></p>
			<p><a href=demo.php?{$lang_}>{$i18['DEMO']}</a></p>
			<p><table cellpadding="0" cellspacing="0">
				<tr align=center><td><a href=order.php?{$lang_}>{$i18['ORDER']}</a></td>
				<td><span align="center">&nbsp;
				<img src="img/paypal.png" alt="PayPal"></span></td></tr>
			</table></p>
		</td>
	</tr>
NAV5;
	
echo <<<NAV6
	<tr valign=top><td></td></tr>
</table>
<small>Execution took $exec_time msec.</small>
</body>
</html>
NAV6;
}
?>
