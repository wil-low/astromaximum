<?php
include_once('lang.php');
unset($LOGIN_MSG);

function emit_nav1(){
	global $lang_, $i18, $LOGIN_MSG;

echo <<<NAV1
<table border="1" width="100%" height=100%>
	<tr height=35%>
		<td width="22%" align=center valign=top>
			<img src="/img/logo.png" border="0" alt="Astromaximum logo">
			<table style="font-size:smaller;" width=100%><tr align=center>
			<td><a href="index.php?lang=en">English</a></td>
			<td><a href="index.php?lang=ru">Русский</a></td>
			</tr></table>
			<br><br>
			<a href=index.php?{$lang_}>{$i18['MAIN']}</a><br><br>
			<a href=feat.php?{$lang_}>{$i18['FEAT']}</a><br><br>
			<a href=scr.php?{$lang_}>{$i18['SCR']}</a><br><br>
			<a href=req.php?{$lang_}>{$i18['REQ']}</a><br><br>
			<a href=contact.php?{$lang_}>{$i18['CONTACT']}</a><br><br>
			<a href=links.php?{$lang_}>{$i18['LINKS']}</a><br><br>
		</td>
		<td rowspan=3 valign=top>
NAV1;
	if(isset($_POST['user'])){
		if(login($_POST['user'],$_POST['passwd'])){
		}
		else{
			$LOGIN_MSG='INVALID_LOGIN';
		}
		unset($_POST['user']);
	}
}

function emit_nav2(){
	global $lang_, $i18, $LOGIN_MSG;

echo <<<NAV2
		</td>
	</tr>
	<tr>
	<td height=20%>
	<center>
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
	<h4>{$i18['MEM_LOGIN']}</h4>
	<span class=login>
	<form method='post' action={$_SERVER['SCRIPT_NAME']}?{$lang_}>
		<table cellpadding=0 border=0>
		<tr><td><font size=-1>{$i18['USERNAME']}</font></td><td><input type="text" name="user"></input></td></tr>
		<tr><td><font size=-1>{$i18['PWD']}</font></td><td><input type="password" name="passwd"></input></td></tr>
		<tr><td colspan=2 align=center><input type=submit value={$i18['LOG_IN']}></input></td></tr>
	</table>
	</form></span>
	<center>
NAV4;
}

if(isset($LOGIN_MSG)){
	echo $i18[$LOGIN_MSG];
	unset($LOGIN_MSG);
}

echo "&nbsp;</center></font>";

echo <<<NAV5
	</center>
	</td></tr>
	<tr align=center valign=top>
		<td>
			<p><br><a href=test.php?{$lang_}>{$i18['TEST']}</a></p>
			<p><a href=demo.php?{$lang_}>{$i18['DEMO']}</a></p>
			<p><table cellpadding="0" cellspacing="0">
				<tr align=center><td><a href=order.php?{$lang_}>{$i18['ORDER']}</a></td>
				<td><span align="center">&nbsp;
				<img src="/img/paypal.png" alt="PayPal"></span></td></tr>
			</table></p>
			<p><a href=geo.php?{$lang_}>{$i18['DB']}</a></p>
		</td>
	</tr>
</table>
</body>
</html>
NAV5;
}
?>
