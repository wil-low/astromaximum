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
        <td width="7" background="img/lin-vert.gif"><img src="../../img/spacer.gif" width="7" height="8"></td>
        <td valign="top"><table width="1003" height="100%"  border="0" cellpadding="0" cellspacing="0">
          <tr>
            <td colspan="2" valign="top">
            <img src="../../img/spacer.gif" width="1" height="10">                
                <table width="220" border="0" cellspacing="0" cellpadding="0">
                  <tr align="left" valign="top">
                    <td height="20" colspan="3"><img src="../../img/ecke.gif" width="20" height="20"><img src="../../img/ecke-gor.gif" width="199" height="4" align="top"><img src="../../img/ecke-right.gif" width="1" height="20"></td>
                    </tr>
                  <tr>
                    <td width="4" rowspan="2" background="../../img/ecke-vert.gif"><img src="../../img/spacer.gif" width="4" height="1"></td>
                    <td class="txt-ramka" align="left">
                    <table border="0">
                    <tr>
                    	<td width="100">
												<img src="img/europe.png" alt="Europe" height="88" width="88" border="1">
											</td>
											<td>
												<div align=center>Europe:</div>
												<a href=# class="nav">Nothern</a><br><a href=# class="nav">Western</a><br>
												<a href=# class="nav">Southern</a><br><a href=# class="nav">Eastern</a><br>
											</td></tr>										
                    <tr>
                    	<td width="25%">
												<img src="img/america.png" alt="America" height="88" width="88" border="1">
											</td>
											<td>
												<div align=center>America:</div>
												<a href=# class="nav">Nothern</a><br><a href=# class="nav">Central</a><br>
												<a href=# class="nav">Southern</a><br><a href=# class="nav">Caribbeans</a><br>
											</td></tr>
                    <tr>
                    	<td width="25%">
												<img src="img/asia1.png" alt="Asia" height="88" width="88" border="1">
											</td>
											<td>
												<div align=center>Asia:</div>
												<a href=# class="nav">Western</a><br><a href=# class="nav">Central</a><br>
												<a href=# class="nav">Southern</a>
											</td></tr>										
                    <tr>
                    	<td width="25%">
												<img src="img/asia2.png" alt="Asia" height="88" width="88" border="1">
											</td>
											<td>
												<div align=center>Asia:</div>
												<a href=# class="nav">Eastern</a><br><a href=# class="nav">Southeastern</a><br>
											</td></tr>										
                    <tr>
                    	<td width="25%">
												<img src="img/australia.png" alt="Australia" height="88" width="88" border="1">
											</td>
											<td>
												<div align=center>Australia:</div>
												<a href=# class="nav">Southeastern Asia</a><br><a href=# class="nav">Australia</a><br>
												<a href=# class="nav">Polinesia</a>
											</td></tr>										
                    <tr>
                    	<td width="25%">
												<img src="img/africa.png" alt="Africa" height="88" width="88" border="1">
											</td>
											<td>
												<div align=center>Africa:</div>
												<a href=# class="nav">Nothern</a><br><a href=# class="nav">Western</a><br>
												<a href=# class="nav">Middle</a><br><a href=# class="nav">Eastern</a><br>
												<a href=# class="nav">Southern</a>
											</td></tr>
											</table>
											</td>
                    <td width="1" rowspan="2" bgcolor="#000000"><img src="../../img/spacer.gif" width="1" height="1"></td>
                  </tr>
                  <tr>
                    <td valign="bottom"><img src="../../img/black.gif" width="215" height="1"></td>
                  </tr>
                </table>
                
NAV1;


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
