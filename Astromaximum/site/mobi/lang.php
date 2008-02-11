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
?>