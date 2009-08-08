<?php
if(!isset($EXEC)) die("Access restricted");
$perl=find_perl();
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
$languages=array('en', 'ru');
if(!isset($_POST['l']) || !in_array($_POST['l'], $languages)){
// ask a language
    $subtitle='Select Astromaximum language';
    echo '<form action="'.htmlentities($_SERVER['REQUEST_URI']).'" method="post"><p>';
    foreach($languages as $value){
        echo "<input type=\"radio\" name=\"l\" value=\"$value\"/>$value ";
    }
?>
<br/><br/><input type="submit"/></p>
</form>
<?php
    return;
}
else{
    $lang=$_POST['l'];
}

$default_city_ids=get_default_cities($GLOBALS['amax']['def_cities'][$lang]);

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
	$cmd="$perl dl/gen_amax.cgi demo $year $lang $default_city_ids $dsrc/$fn.r $timeout_offset $timeout_mins";
}
else{
	if(!$user_ok) return; 
	$cmd="$perl dl/gen_amax.cgi tb $year $lang \"$default_city_ids\" $dsrc/$fn.r $timeout_offset $timeout_mins";
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

$data_php=$_SERVER['SERVER_NAME'];
if(strpos($data_php, "mobi") === false){
	$data_php.="/mobi";
}
//echo "http://$data_php/../data.php?t=$fn";
echo "<a href=\"http://$data_php/data.php?t=$fn\">{$i18['PHONE_DL']}</a>";
echo "<br/><span class=\"fine\">{$i18['VALID_LINKS']}</span><br/><br/>";

 
?>
