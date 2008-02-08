<?php
include_once('../lang.php');
include_once('../dbconnect.php');
$invalid_login=0;

$langs=array(
	"en"=>'English', 
//	"de"=>'Deutsch', 
	"ru"=>'Русский'
);

function emit_admin(){
	global $lang_, $i18;
	echo <<<ADMIN
	 &nbsp; <b>Admin:</b> <a href='index.php'>Download</a> 
        <a href='db_stats.php'>{$i18['DB_STATS']}</a> 
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
	global $lang_, $i18, $invalid_login, $START_TIME, $langs;
	$START_TIME=microtime_float();
// entrance for clients
	if(isset($_POST['user']) && isset($_POST['passwd'])){
		if(login($_POST['user'],$_POST['passwd'])){
			$invalid_login=0;
		}
		else{
			$invalid_login=1;
		}
		unset($_POST['user']);
	}
	if(isset($_GET['lang'])){
		$lng=$_GET['lang'];
	}
	else{
		$lng='en';
	} 
if($invalid_login || ($_SESSION['uid']==-1)){
    echo <<<NAV1
    <form action="{$_SERVER['REQUEST_URI']}" method="post" name="log">
    <table align=center>
    <tr align=center>
        <td>{$i18['USERNAME']}</td><td><input name="user" type="text" size="15" maxlength="15"></td>
    </tr>
    <tr align=center>
        <td>{$i18['PWD']}</td><td><input name="passwd" type="password" size="15" maxlength="15"></td>
    </tr>
    <tr align=center>
        <td colspan=2><input type="submit" value="Авторизация"></td>
    </tr></table>
    </form>
NAV1;
}
else{
    echo "Welcome, {$_SESSION['username']}! (<a href='logout.php'>logout</a>)<br><br>";
}
/*
print_r($_POST);
echo "<br>";
print_r($_SESSION);
*/
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
