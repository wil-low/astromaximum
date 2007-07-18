<?php
$lang='en';
if(isset($_GET['lang'])){
	$lang=$_GET['lang'];
}
$fd = fopen("../source/$lang/lang.txt", 'r');
while (!feof($fd)) {
	$buffer = fgets($fd, 4096);
	list($key,$value)=explode("=",$buffer);
	$i18[$key]=trim($value);
}
fclose($fd);
$lang_="lang=$lang";
error_reporting(E_ALL);
?>