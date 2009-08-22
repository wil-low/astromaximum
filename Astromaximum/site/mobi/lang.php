<?php
if(!isset($EXEC)) die("Access restricted");
if(!isset($lang))
    $lang=get_preferred_lang();
$i18=array();
if(isset($_REQUEST['lang'])){
	$lang=strtolower($_REQUEST['lang']);
}
$lang_="lang=$lang";

// We are starting to offer next year after October 22 inclusively (God's world creation)
$next_at='1022'; // MMDD

$current_year=date("Y");
if(strcmp(date("md"),$next_at)>=0){
    $current_year++;
};
$GLOBALS['amax']['year']=$current_year;

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
	return "<a href=\"$pp\">";
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

# taken from http://www.thefutureoftheweb.com/blog/use-accept-language-header
function get_preferred_lang()
{
	$link_lang = 'en';
	$langs = array();

	if (isset($_SERVER['HTTP_ACCEPT_LANGUAGE'])) {
		// break up string into pieces (languages and q factors)
		preg_match_all('/([a-z]{1,8}(-[a-z]{1,8})?)\s*(;\s*q\s*=\s*(1|0\.[0-9]+))?/i', 
			$_SERVER['HTTP_ACCEPT_LANGUAGE'], $lang_parse);

		if (count($lang_parse[1])) {
			// create a list like "en" => 0.8
			$langs = array_combine($lang_parse[1], $lang_parse[4]);
			
			// set default to 1 for any without q factor
			foreach ($langs as $lang => $val) {
				if ($val === '') $langs[$lang] = 1;
			}

			// sort list based on value	
			arsort($langs, SORT_NUMERIC);
		}
	}

	// look through sorted list and use first one that matches our languages
	foreach ($langs as $lang => $val) {
		if (preg_match('/(ru|uk|by)/', $lang)) {
			$link_lang = 'ru';
			break;
		}
		if (strpos($lang, 'en') === 0) {
			$link_lang = 'en';
			break;
		}
	}
	return $link_lang;
}
?>
