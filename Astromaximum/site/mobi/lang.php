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
	echo "<a href=\"?lang=$lang&amp;p=$pp\">";
} 

function dload_prompt($str){
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	echo <<<EOF
<script type="text/javascript">
<!--
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
-->
</script>
<form action="$uri" method="post" onsubmit="checkCheckBox(this)">
<input type="checkbox" name="agree" style="width:auto;"/> {$str}<br/><br/>
<input type="submit" style="height:auto;" value="OK"/>
</form>
EOF;
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