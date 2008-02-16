<?php
$lang='ru';
$i18=array();
if(isset($_GET['lang'])){
	$lang=strtolower($_GET['lang']);
}
$lang_="lang=$lang";
error_reporting(E_ALL);
session_name("Astromaximum");
session_start();
session_register("username","uid", "pwd");

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
	echo "<a href=\"?lang=$lang&p=$pp\">";
} 

function dload_prompt($str){
	echo <<<EOF
<script type="text/javascript">
function checkCheckBox(f){
	if(f.agree.checked==false)
	{
		alert('Please check the box to continue.');
		return false;
	}
	else{
		return true;
	}
}
</script>
<form action="{$_SERVER['REQUEST_URI']}" method="post" onsubmit="checkCheckBox(this)">
<input type="checkbox" name="agree"> {$str}<br/><br/>
<input type="submit" value="OK">
</form>
EOF;
} 

function reg_warning($subj){
	echo "<p>$subj разрешается только зарегистрированным пользователям.<br/>Введите свой логин и пароль в форме слева.";
}

?>