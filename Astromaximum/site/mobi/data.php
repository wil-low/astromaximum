<?php
$EXEC=5;
include_once('config.php');
include_once('dbconnect.php');
$PREFIX='Cities';
if(true /*|| check_access()*/){
    $type='';
	if(isset($_GET['r'])){
		$type='r';
	}
	if(isset($_GET['d'])){
		$type='d';
	}
	if(isset($_GET['t'])){
		$type='t';
	}
    if(!isset($_GET[$type]))
        data_gone();
	$dig=$_GET[$type];
	$idd=substr($dig, -4);
	$ye=substr($dig, 0,2);
	$fn="$DIR_FILES/".$dig.".$type";
	$stat=sprintf(
		"SELECT type FROM files WHERE id='%s' AND end_tm>NOW() AND NOT deleted", quote_smart($dig));
	$sth=mysql_query($stat);
	if(mysql_affected_rows() != 1)
        data_gone();
	$row=mysql_fetch_row($sth);
    if(strcmp($row[0]{0}, 'g') != 0){ // not a geoAM
        $PREFIX = 'Amax';
    }
    
	$stat=sprintf(
		"UPDATE files SET used='t' WHERE id='%s'", quote_smart($dig));
	mysql_query($stat);
	if(strcmp($type,'t')==0){
		$type='d';
	}

	$handle = fopen($fn, "rb");
	if(!$handle)
        data_gone();
	$clen=filesize($fn);
	$data = fread($handle, $clen);
	fclose($handle);
	if(strcmp($type,'d')==0){
		header('Content-Type: text/vnd.sun.j2me.app-descriptor; charset=UTF-8');
	}
	else{
//		echo filesize($fn); 
		header('Content-Type: application/java-archive');
	}
	header("Content-Length: $clen");
	header("Content-Disposition: attachment; filename=\"$PREFIX'$ye-$idd.ja$type\"", false);
	echo $data;
}

function data_gone(){
    header("HTTP/1.0 410 Gone");
    echo "Sorry, wrong URL or no file present.";
    exit;
}
?>
