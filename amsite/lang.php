<?php
include_once('../dbconnect.php');
$lang='';
$i18=array();
if(isset($_GET['lang'])){
	$lang=$_GET['lang'];
}
else{
	$lang='en';
}
$fd = fopen("source/$lang/lang.txt", 'r');
while (!feof($fd)) {
	$buffer = fgets($fd, 4096);
	list($key,$value)=explode("=",$buffer);
	$i18[$key]=trim($value);
}
fclose($fd);
$lang_="lang=$lang";
error_reporting(E_ALL);
session_start();

if(!isset($_SESSION['username'])){
	$_SESSION['username']='nobody';
	$_SESSION['uid']=0;
}

?>