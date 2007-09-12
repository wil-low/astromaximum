<?php
//include_once('lang.php');
include_once('dbconnect.php');
if(true /*|| check_access()*/){
	if(isset($_GET['r'])){
		$type='r';
	}
	if(isset($_GET['d'])){
		$type='d';
	}
	if(isset($_GET['t'])){
		$type='t';
	}
	$dig=$_GET[$type];
	$idd=substr($dig, -4);
	$fn="$DIR_FILES/".$dig.".$type";
	$stat=sprintf(
		"UPDATE files SET used='t' WHERE id=%s AND type='%s'", $dig, $type);
	mysql_query($stat);
	if(strcmp($type,'t')==0){
		$type='d';
	}

	$handle = fopen($fn, "rb");
	$data = fread($handle, filesize($fn));
	fclose($handle);
	if(strcmp($type,'d')==0){
		header('Content-type: text/vnd.sun.j2me.app-descriptor');
	}
	else{
//		echo filesize($fn); 
		header('Content-type: application/java-archive');
	}
	header('Content-Disposition: attachment; filename="Cities-'.$idd.'.ja'.$type.'"', false);
	echo $data;

}
?>
