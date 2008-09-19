<?php
if(!isset($EXEC)) die("Access restricted");
$lang='ru';
$i18=array();
if(isset($_REQUEST['lang'])){
	$lang=strtolower($_REQUEST['lang']);
}
$lang_="lang=$lang";
$GLOBALS['amax']['year']=get_year();

function lang_load($path){
	global $lang, $i18;
	if(!preg_match("/^\w\w$/", $lang)){
		return;
	}
	if(!file_exists("$path/$lang.msg")){
		return;
	}
	$fd = fopen("$path/$lang.msg", 'r');
	while (!feof($fd)) {
		$buffer = fgets($fd, 4096);
		$line=explode("=",$buffer,2);
		if(count($line)==2){
			list($key,$value)=$line;
			$i18[$line[0]]=trim($line[1]);
		}
	}
	fclose($fd);
}

function anchor($pp){
	global $lang;
	return "<a href=\"?lang=$lang&amp;p=$pp\">";
} 

function dload_prompt($str, $is_disabled){
	$ret='';
	$disabled='';
	$cls="ok_on";
	if($is_disabled){
		$disabled=" disabled=\"disabled\"";
		$cls="ok_off";
		$onclick='';
	}
	else{
		$onclick=' onclick="checkCheckBox(this)"';
	}
	$ret.='<input type="checkbox" name="agree" style="width:auto;"'.$disabled.'/> '.$str.'<br/><br/>'.
	'<input type="button" class="'.$cls.'" value="OK"'.$onclick.$disabled.'/>';
	return $ret;
} 

function reg_warning($subj){
	global $i18;
	echo "<h4>{$i18['REG_REQ']}</h4>".'<p>'.sprintf($i18['RESTRICTED'], $subj).'</p>';
}

function get_year(){
	$current_year=date("Y");
	if(date("n")==12){
		$current_year++;
	};
	return $current_year;
}
?>
