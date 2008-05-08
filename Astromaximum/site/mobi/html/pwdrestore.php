<?php
if(!isset($EXEC)) die("Access restricted");
echo "<h4>Восстановление регистрации</h4>";
include_once("mobi/ipblock.php");
include_once("mobi/amtools.php");
$msg=allow_ip('pwd_rest', false);
echo $msg;
if($msg) return;
if(isset($_POST['p_email']) && isset($_POST['p_captcha'])){
	$mail=$_POST['p_email']; $captcha=$_POST['p_captcha'];
	if(is_captcha($captcha)){
		$arr=email2login($mail);
		if(count($arr)){ 
			$newpass=sprintf("%09d", mt_rand(1, 999999999));
			include_once("mobi/amtools.php");
			$tries=get_try_count($arr[0]);
			$mail=pwd_send($mail, $arr[1], $arr[2], $tries, $newpass);
			if($mail->Send()){
				echo "New password has been sent to email address specified.";
				$pwd=pwd_convert2(pwd_convert1($arr[1], $newpass));
				$stat=sprintf("UPDATE customers set hash=%s WHERE id=%d",
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
		echo "<p><font color=\"red\">{$i18['CAPTCHA_WRONG']}</font></p>";
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
<p><?php echo $i18['CAPTCHA_PROMPT']?></p>
<p><img src="mobi/kcaptcha?<?php echo session_name()?>=<?php echo session_id()?>" alt="Captcha">
<input name="p_captcha" type="text"/>
</p>
<input name="action" type="button" class="ok_on" value="OK" onclick="checkdata()"/>
</form>
<?php
function email2login($mail){
	$arr=array();
	if(strlen($mail)>0){
		$stat=sprintf("SELECT id, name, realname FROM customers WHERE email=%s AND active>0",
			quote_smart($mail));
//		echo $stat;
		$sth=mysql_query($stat);
		if(mysql_num_rows($sth)==1){
			$arr=mysql_fetch_array($sth);
		}
	}
	return $arr;
}
?>