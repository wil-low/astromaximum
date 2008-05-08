<?php
if(!isset($EXEC)) die("Access restricted");
$perl=find_perl();
$default_city_ids=get_default_cities($GLOBALS['amax']['def_cities']);
if(!isset($_REQUEST['mode'])) exit;
$year=$GLOBALS['amax']['year'];
$isdemo=0;
if(strcmp($_REQUEST['mode'], 'demo')==0){
	$year--;
	$isdemo=1;
}
else if(strcmp($_REQUEST['mode'], 'trial')!=0){
	exit;
}
$timeout_offset=-24;
$timeout_mins=2880;  

$outp=array();
global $DIR_FILES, $DIR_SOURCE;
$dsrc=$DIR_FILES;
$ye=substr($year,-2);
list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
$srcdir="$dsrc/$fn";
#	echo "$dsrc/$destfile";
$type="d $year $lang";
if($isdemo){
	$cmd="$perl dl/gen_amax.cgi demo $year $lang $default_city_ids $dsrc/$fn.r $timeout_offset $timeout_mins nomessjar";
}
else{
	if(!$user_ok) return; 
	$cmd="$perl dl/gen_amax.cgi tb $year $lang \"$default_city_ids\" $dsrc/$fn.r $timeout_offset $timeout_mins nomessjar";
	$type="t $year $lang";
}
$ret=0;
exec($cmd, $outp, $ret);
//	$ret=1;
if($ret){				
	echo $cmd;
	echo implode('<br>',$outp);
	exit;
}
if(!add_file($fn, $type)){
	echo mysql_error();
	exit;
}

$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
if(!strpos($data_php, "mobi")){
	$data_php.="/mobi";
}
//echo "http://$data_php/../data.php?t=$fn";
echo "<a href=\"http://$data_php/data.php?t=$fn\">{$i18['PHONE_DL']}</a>";
echo "<br/><font color='red'>{$i18['VALID_LINKS']}</font><br/><br/>";

 
?>
