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
$str = midlet_create ($isdemo? 'demo' : 'tb', $year, $lang, $default_city_ids, 'dl', 0);
if (!strlen($str))
	return;
$data_php=$_SERVER['SERVER_NAME'];
if(strpos($data_php, "mobi") === false){
	$data_php.="/mobi";
}
//echo "http://$data_php/../data.php?t=$fn";
echo "<a href=\"http://$data_php/data.php?t=$str\">{$i18['PHONE_DL']}</a>";
echo "<br/><span class=\"fine\">{$i18['VALID_LINKS']}</span><br/><br/>";

?>
