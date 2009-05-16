<?php 
if (!isset($EXEC)) die("Access restricted");
$mode = 'main'; 
if (isset ($_GET['m']))
	$mode = $_GET['m'];
$cfields = array();

include_once ('mobi/html/c_paid.php');
if (!isset($cfields[$mode]))
	return;
$sess=session_name().'='.session_id();
$agree=dload_prompt(sprintf($i18['CONFIRM_TRIAL'], $lang, $lang), false);
$arr = $cfields[$mode];
$field_count = count ($arr);
echo "<h4>{$arr['header']}</h4>\n";
if (isset ($_POST['agree']) && strcmp ($_POST['agree'], 'on') == 0) {
	$msg = ''; $content = '';
	do {
		if(!is_captcha($_POST['p_captcha'])){
		    $msg=alert($i18['CAPTCHA_WRONG']); break;
		}
		for ($i = 0; $i < $field_count; ++$i) {
			$key = sprintf('f%02d', $i);
			$caption = $i18[$arr['captions'][$i]];
			if (!$_POST[$key]) {
				$msg = alert (sprintf($i18['MISSING_FIELD'], $caption)); 
				break;
			}
			else {
				$content .= "$caption:\t".$_POST[$key]."\n";
			}
		}
	} while (0);
	if ($msg)
		echo $msg;
	else {
		call_user_func ('cform_'.$mode, $content);
		return;
	}
}
$out = '<form id="regform" action="'.$_SERVER['REQUEST_URI'].'" method="post">'."\n<table>";
for ($i = 0; $i < $field_count; ++$i) {
	$out .= sprintf('<tr><td>%s</td><td><input type="text" name="f%02d"/></td></tr>', 
		$i18[$arr['captions'][$i]], $i)."\n";
}
echo $out;
echo <<< EOF
</table>
<input type="hidden" name="demo"/>
<p><img src="/mobi/kcaptcha?$sess" alt="Captcha"/>
<input type="text" name="p_captcha" size="8"/></p>
$agree
</form>
EOF;

function alert($str){
    return '<span class="alert">'.$str.'</span>';
}

?>


