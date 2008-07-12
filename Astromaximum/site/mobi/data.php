<?php
$EXEC=5;
include_once('dbconnect.php');
$PREFIX='Cities';
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
	$ye=substr($dig, 0,2);
	$fn="$DIR_FILES/".$dig.".$type";
	$stat=sprintf(
		"SELECT COUNT(*) FROM files WHERE id='%s' AND end_tm>NOW() AND NOT deleted", quote_smart($dig));
	$sth=mysql_query($stat);
	$count=mysql_fetch_row($sth);
	if($count[0]!=1){
		header("HTTP/1.0 410 Gone");
		echo "Sorry, wrong URL or no file present.";
		exit;
	}
	$stat=sprintf(
		"UPDATE files SET used='t' WHERE id='%s'", quote_smart($dig));
	mysql_query($stat);
	if(strcmp($type,'t')==0){
		$type='d';
	}

	$handle = fopen($fn, "rb");
	$clen=filesize($fn);
	$data = fread($handle, $clen);
	fclose($handle);
	if(strcmp($type,'d')==0){
		header('Content-Type: text/vnd.sun.j2me.app-descriptor;charset=UTF-8');
	}
	else{
//		echo filesize($fn); 
		header('Content-Type: application/java-archive');
	}
	header("Content-Length: $clen");
	header("Content-Disposition: attachment; filename=\"$PREFIX'$ye-$idd.ja$type\"", false);
	echo $data;

}
?>
