<?php
$lang='';
$i18=array();
if(isset($_GET['lang'])){
	$lang=$_GET['lang'];
}
else{
	$lang='en';
}
$lang_="lang=$lang";
error_reporting(E_ALL);
session_start();

if(!isset($_SESSION['username'])){
	$_SESSION['username']='';
	$_SESSION['uid']=-1;
	$_SESSION['pwd']='*';
}

function lang_load($path){
	global $lang, $i18;
	$fd = fopen("$path/$lang/lang.txt", 'r');
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