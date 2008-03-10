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
	$message = <<<EOF
Dear $realname,

You requested a new password for access to http://astromaximum.de 
Your credentials is now as follows:
	login:     $login
	password:  $pwd
	
Number of Astromaximum copies to download: $dl_count
Number of cities to download: $city_count

This mail was generated automatically, there is no need to reply.

Thank you for using our service.

-----
Уважаемый $realname,

Вы запросили новый пароль для доступа на сайт http://astromaximum.de
Для входа на сайт наберите:
	логин:   $login
	пароль:  $pwd

Разрешено загрузить копий мидлета на текущий год: $dl_count
Разрешено загрузить городов: $city_count

Это письмо сгенерировано автоматически, нет нужды отвечать на него.

Спасибо за использование нашего сервиса.
EOF;
	$headers  = 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/plain; charset=UTF-8' . "\r\n";	
	$headers .= 'From: robot@astromaximum.de' . "\r\n" .
	    'X-Mailer: PHP';
	return mail($to, $subject, $message, $headers);
}
?>