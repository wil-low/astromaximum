<h4>Восстановление регистрации</h4>
<?php
if(isset($_POST['p_email']) && isset($_POST['p_captcha'])){
	$mail=$_POST['p_email']; $capcha=$_POST['p_captcha'];
	if(is_capcha($capcha)){
		$arr=email2login($mail);
		if(count($arr)){ 
			$newpass=sprintf("%09d", mt_rand(1, 999999999));
			if(pwd_send($mail, $arr[0], $arr[1], $arr[2], $arr[3], $newpass)){
				echo "New password has been sent to email address specified.";
				$pwd=pwd_convert2(pwd_convert1($arr[0], $newpass));
				$stat=sprintf("UPDATE customers set hash=%s WHERE name=%s",
					quote_smart($pwd), quote_smart($arr[0]));
				if(!mysql_query($stat)){
					echo "Error setting password:<br/>$stat<br/>".mysql_error();
				}
			}
		}
		else{
			sleep(5);
			echo "Email is not found in database: <b>'$mail'</b>";
			echo "<p><a href=\"{$_SERVER['REQUEST_URI']}\">back</a></p>";
		}
		return;
	}
	else{
		sleep(5);
		echo "<p><font color=\"red\">Invalid string entered</font></p>";
	}
} 
?>
<script type="text/javascript">
<!--
	function checkdata(){
		frm=findObj('pwdrestore');
		frm.submit();
	}
-->
</script>
<p>Введите e-mail, указанный Вами при регистрации. На него будут высланы Ваши логин и новый пароль:</p>
<form id="pwdrestore" action="<?php echo $_SERVER['REQUEST_URI']?>" method="post">
<input name="p_email" type="text" style="width: auto"/>
<p>Введите символы, указанные на рисунке:</p>
<p><img src="mobi/kcaptcha?<?php echo session_name()?>=<?php echo session_id()?>">
<input name="p_captcha" type="text"/>
</p>
<input name="action" type="button" value="OK" onclick="checkdata()"/>
</form>
<?php
function email2login($mail){
	$arr=array();
	if(strlen($mail)>0){
		$stat=sprintf("SELECT name, realname, dl_count, city_count FROM customers WHERE email=%s AND active>0",
			quote_smart($mail));
//		echo $stat;
		$sth=mysql_query($stat);
		if(mysql_num_rows($sth)==1){
			$arr=mysql_fetch_array($sth);
		}
	}
	return $arr;
}

function is_capcha($capcha){
	$res=false;
	if(count($_POST)>0){
		$res=isset($_SESSION['captcha_keystring']) && ($_SESSION['captcha_keystring'] ==  $_POST['p_captcha']);
	}
	unset($_SESSION['captcha_keystring']);
	return $res;
}

function pwd_send($to, $login, $realname, $dl_count, $city_count, $pwd){
	$subject = 'Astromaximum.de - new password';
/*	$message = <<<EOF
<html><head>
  <title>Astromaximum.de - new password</title>
</head>
<body>
<p>Dear $realname,</p>

<p>You requested a new password for access to 
<a href="http://astromaximum.de/">http://astromaximum.de/</a> 
<br/>Your credentials are now as follows:</p>
<ul>
<li>login: $login</li>
<li>password:  $pwd</li>
</ul>	
<p>Number of Astromaximum copies to download: $dl_count</p>
<p>Number of cities to download: $city_count</p>

<p>This mail was generated automatically, there is no need to reply.</p>

<p>Thank you for using our service.</p>
<hr/>
EOF;

$rusmsg= <<<EOF1
<p>Уважаемый $realname,</p>

<p>Вы запросили новый пароль для доступа на сайт 
<a href="http://astromaximum.de/">http://astromaximum.de/</a>
<br/>Для входа на сайт наберите:</p>
<ul>
<li>логин:   $login</li>
<li>пароль:  $pwd</li>
</ul>
<p>Разрешено загрузить копий мидлета на текущий год: $dl_count</p>
<p>Разрешено загрузить городов: $city_count</p>

<p>Это письмо сгенерировано автоматически, нет нужды отвечать на него.</p>

<p>Спасибо за использование нашего сервиса.</p>
EOF1;
*/
	$message=file_get_contents("mobi/dl/source/pwdrestore.mail");
	$message=str_replace('<login>', $login, $message);
	$message=str_replace('<pwd>', $pwd, $message);
	$message=str_replace('<dl_count>', $dl_count, $message);
	$message=str_replace('<city_count>', $city_count, $message);
	$headers = 'From: robot@astromaximum.de' . "\r\n" .
	    'X-Mailer: PHP';
	return mail($to, $subject, $message, $headers);
}
?>