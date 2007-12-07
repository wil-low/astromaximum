<?php
include_once('../lang.php');
unset($LOGIN_MSG);

$langs=array(
	"en"=>'English', 
//	"de"=>'Deutsch', 
	"ru"=>'Русский'
);

function emit_admin(){
	global $lang_, $i18;
	echo <<<ADMIN
	 &nbsp; <b>Admin:</b> <a href='db_stats.php'>{$i18['DB_STATS']}</a> 
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
	global $lang_, $i18, $LOGIN_MSG, $START_TIME, $langs;
	$START_TIME=microtime_float();
// entrance for clients
	if(isset($_POST['user']) && isset($_POST['passwd'])){
		if(login($_POST['user'],$_POST['passwd'])){
		}
		else{
			$LOGIN_MSG='INVALID_LOGIN';
		}
		unset($_POST['user']);
	}
	if(isset($_GET['lang'])){
		$lng=$_GET['lang'];
	}
	else{
		$lng='en';
	} 
echo <<<NAV1
<table width="1013" border="0" align="center" cellpadding="1" cellspacing="0" bgcolor="#000000">
  <tr>
    <td valign="top"><table width="1011" height="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
      <tr>
        <td height="127" colspan="2" valign="top"><table width="100%" height="127"  border="0" cellpadding="0" cellspacing="0">
          <tr>
            <td><img src="../../img/top1.gif" width="44" height="120"></td>
            <td><img src="../../img/top2.gif" width="545" height="120"></td>
            <td><img src="../../img/top3.jpg" width="248" height="120"></td>
            <td><img src="../../img/top4.jpg" width="175" height="120"></td>
          </tr>
          <tr align="left" valign="top">
            <td colspan="4"><img src="../../img/lin-goris.gif" width="1011" height="7"></td>
          </tr>
        </table></td>
      </tr>
      <tr>
        <td width="7" background="img/lin-vert.gif"><img src="../../img/spacer.gif" width="7" height="8"></td>
        <td valign="top"><table width="1003" height="100%"  border="0" cellpadding="0" cellspacing="0">
          <tr>
            <td width="237" height="30" align="right" valign="middle" class="gr-1">
            <a href="{$_SERVER['PHP_SELF']}?">DE</a> 
            <a href="{$_SERVER['PHP_SELF']}?lang=en">ENG</a> 
            <a href="{$_SERVER['PHP_SELF']}?lang=ru">RU</a>
            </td>
            <td width="766" align="right" valign="middle"><img src="../../img/sonne-kl.gif" width="12" height="11" align="middle"> &nbsp;&nbsp;&nbsp;<img src="../../img/mond-kl.gif" width="12" height="11" align="middle">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
              <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0" width="70" height="20" align="middle">
                <param name="movie" value="img/klock.swf">
                <param name="quality" value="high">
                <embed src="../../img/klock.swf" width="70" height="20" align="middle" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash"></embed>
              </object>
              &nbsp;&nbsp;</td>
          </tr>
          <tr>
            <td colspan="2"><table width="100%"  border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="17"><img src="../../img/spacer.gif" width="17" height="1"></td>
                <td valign="top"><table width="220" border="0" cellspacing="0" cellpadding="0">
                  <tr align="left" valign="top">
                    <td height="20" colspan="3"><img src="../../img/ecke.gif" width="20" height="20"><img src="../../img/ecke-gor.gif" width="199" height="4" align="top"><img src="../../img/ecke-right.gif" width="1" height="20"></td>
                  </tr>
                  <tr>
                    <td width="4" rowspan="2" background="../../img/ecke-vert.gif"><img src="../../img/spacer.gif" width="4" height="1"></td>
                    <td class="txt-ramka" align="left">
NAV1;
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
                    <form action="geo.php?lang={$lng}" method="post" name="log">
                      &nbsp;<input name="user" type="text" size="15" maxlength="15">&nbsp;{$i18['USERNAME']}<br>
                      &nbsp;<input name="password" type="password" size="15" maxlength="15">&nbsp;{$i18['PWD']}
                      <p align="center">
                      <input type="submit" value="Авторизация">
<!--   <a href="{$_SERVER['REQUEST_URI']}" class="red" onclick="javascript: form.submit()">Авторизация</a> -->
</p></form>
NAV4;
}
if(isset($LOGIN_MSG)){
	echo $i18[$LOGIN_MSG];
	unset($LOGIN_MSG);
}
// entrance for clients end
echo <<<NAV1_1
                    </td>
                    <td width="1" rowspan="2" bgcolor="#000000"><img src="../../img/spacer.gif" width="1" height="1"></td>
                  </tr>
                  <tr>
                    <td valign="bottom"><img src="../../img/black.gif" width="215" height="1"></td>
                  </tr>
                </table><img src="../../img/spacer.gif" width="1" height="10">                
                <table width="220" border="0" cellspacing="0" cellpadding="0">
                  <tr align="left" valign="top">
                    <td height="20" colspan="3"><img src="../../img/ecke.gif" width="20" height="20"><img src="../../img/ecke-gor.gif" width="199" height="4" align="top"><img src="../../img/ecke-right.gif" width="1" height="20"></td>
                    </tr>
                  <tr>
                    <td width="4" rowspan="2" background="../../img/ecke-vert.gif"><img src="../../img/spacer.gif" width="4" height="1"></td>
                    <td class="txt-ramka" align="left">&nbsp;
<!--                    <a href="#" class="nav">зМБЧОБС</a><br>
                      &nbsp;<a href="#" class="nav">пРЙУБОЙЕ РТПЗТБННЩ</a><br>
                      &nbsp;<a href="#" class="nav">бУФТП-ЙОУФТХЛГЙЙ</a><br>
                      &nbsp;<a href="#" class="nav">чЩВПТ ЗПТПДБ</a><br>
&nbsp;<img src="../../img/spacer.gif" width="1" height="20"><a href="#" class="nav">уЛБЮБФШ ДЕНП-ЧЕТУЙА</a><br>
&nbsp;<a href="#" class="nav">лХРЙФШ <span class="gr-0">ASTROMAXIMUM</span></a><br>
&nbsp;<img src="../../img/spacer.gif" width="1" height="20"><a href="#" class="nav">юбчП</a><br>
&nbsp;<a href="#" class="nav">лПОФБЛФ</a><br>-->
<p>&nbsp;</p></td>
                    <td width="1" rowspan="2" bgcolor="#000000"><img src="../../img/spacer.gif" width="1" height="1"></td>
                  </tr>
                  <tr>
                    <td valign="bottom"><img src="../../img/black.gif" width="215" height="1"></td>
                  </tr>
                </table>
                
NAV1_1;


if(isset($LOGIN_MSG)){
	echo $i18[$LOGIN_MSG];
	unset($LOGIN_MSG);
}
// entrance for clients end

	echo	"<td valign=top width=\"100%\">";
}

function emit_nav2(){
	global $lang_, $i18, $LOGIN_MSG, $START_TIME;

$exec_time=microtime_float()-$START_TIME;
/*
echo <<<NAV5
	&nbsp;</center></font></td>
	</center>
	</td></tr>
	<tr align=center valign=top>
		<td>
			<p><br><a href=index.php?{$lang_}>{$i18['INSTR']}</a></p>
			<p><a href=geo.php?{$lang_}>{$i18['DB']}</a></p>
			<p><br><a href=test.php?{$lang_}>{$i18['TEST']}</a></p>
			<p><a href=demo.php?{$lang_}>{$i18['DEMO']}</a></p>
			<p><table cellpadding="0" cellspacing="0">
				<tr align=center><td><a href=order.php?{$lang_}>{$i18['ORDER']}</a></td>
				<td><span align="center">&nbsp;
				<img src="../../img/paypal.png" alt="PayPal"></span></td></tr>
			</table></p>
		</td>
	</tr>
NAV5;
*/	
echo <<<NAV6

	<tr valign=top><td></td></tr>
</table>
<small>Execution took $exec_time msec.</small>
</body>
</html>
NAV6;
}
?>
