<?php
if(!isset($EXEC)) die("Access restricted");
include_once('config.php');
$lang='ru';
$i18=array();
if(isset($_REQUEST['lang'])){
	$lang=strtolower($_REQUEST['lang']);
}
$lang_="lang=$lang";

function sess_start(){

	error_reporting(E_NONE);
	#error_reporting(E_ALL);
	
	session_set_cookie_params(3600);
	session_start();
	session_register("username","uid", "pwd", "captcha_keystring");
}

function lang_load($path){
	global $lang, $i18;
	if(!file_exists("$path/$lang.msg")){
		return;
	}
	$fd = fopen("$path/$lang.msg", 'r');
	while (!feof($fd)) {
		$buffer = fgets($fd, 4096);
		$line=explode("=",$buffer);
		if(count($line)==2){
			list($key,$value)=$line;
			$i18[$line[0]]=trim($line[1]);
		}
	}
	fclose($fd);
}

function anchor($pp){
	global $lang;
	echo "<a href=\"?lang=$lang&amp;p=$pp\">";
} 

function dload_prompt($str, $is_disabled){
	$ret='';
	$disabled='';
	if($is_disabled){
		$disabled=" disabled=\"disabled\"";
		$onclick='';
	}
	else{
		$ret = <<<EOF
<script type="text/javascript">
<!--
	function checkCheckBox(b){
		if(b.form.agree.checked==false)
		{
			alert('Please check the box to continue.');
		}
		else{
			b.form.submit();
		}
	}
-->
</script>
EOF;
		 $onclick=' onclick="checkCheckBox(this)"';
	}
	$ret.='<input type="checkbox" name="agree" style="width:auto;"'.$disabled.'/> '.$str.'<br/><br/>'.
	'<input type="button" style="height:auto;" value="OK"'.$onclick.$disabled.'/>';
	return $ret;
} 

function reg_warning($subj){
	echo "<p>$subj разрешается только зарегистрированным пользователям.<br/>Введите свой логин и пароль в форме слева.</p>";
}

function get_year(){
	$current_year=date("Y");
	if(date("n")==12){
		$current_year++;
	};
	return $current_year;
}
?>
