<?php
if(!isset($EXEC)) die("Access restricted");
/*
if($chac==-1 or $chac==1){
	echo '<h3>Page not found</h3>';
	return;
}
*/
$META_TITLE=$i18['TIT_CALENDAR_DOWNLOAD'];

$current_year=$GLOBALS['amax']['year'];
$uri=htmlentities($_SERVER['REQUEST_URI']);

$msg=$i18['REGFORM_REQF'];

$email=$email2=$nick=$paymode=$model='';

if($chac==-1 or $chac == 1 or $chac == 3){ // unpaid
	if ($GLOBALS['amax']['buy_enabled']) {
		echo '<h4>'.$META_TITLE.'</h4><ul>';
		foreach($GLOBALS['amax']['paymodes'] as $key){
			$key2=sprintf('%02d', $key);
			echo '<li><a href="p_'.$key2.'">'.$i18['PAYMENT_'.$key2]."</a><br/><br/></li>\n";
		}
		echo '</ul>';
	}
    return;
}

function alert($str){
    global $i18;
    return alert2 ($i18[$str]);
}

function alert2($str){
    return '<span class="alert">'.$str.'</span>';
}

if($chac!=-1 and $chac!=1){
	$y_now=$current_year;
	if($chac==0 or $chac==2)
		$y_now += 1;
	$out='';
	for($i=$y_now-1; $i>=$GLOBALS['amax']['min_demo_year']; $i--){
		$out.="<option value=\"$i\"";
		if($i==$current_year) $out.=" selected=\"selected\"";
		$out.=">$i</option>\n";
	}
	$year=$y_now;

	$tries=get_try_count(0);

	if ($tries[0] == 0)
		show_payment_instructions(0);
	else {
		global $DLIM;
		$dl_key=0;
		$is_allow_dl=($tries[0]!=0);
		if(isset($_POST["yagree"])){ // past year request
			$year=(int)$_POST["yagree"];
			if($chac!=0 && $year>=$current_year){
				return;
			}
			$is_allow_dl=($tries[2]!=0);
			$dl_key=2;
		}
		echo "<h4>".sprintf($i18['DLOAD4YEAR'], $year)."</h4>";
		if(isset($_POST["agree"])){
			if($is_allow_dl){
				$sc=get_default_cities($GLOBALS['amax']['def_cities'][$lang]);
				echo tries_remained($tries[$dl_key]-1, $DLIM[$dl_key]);
				$str=midlet_create("tb", $year, $lang, $sc, "mobi/dl", 2);
				if(strlen($str)){
					dec_try_count(0, $dl_key);
					echo $str;
				}
			}
			else{
				echo sprintf($i18['NO_CALENDAR_DL'], '<a href="#">');
			}
			echo "<p><a href=\"$uri\">{$i18['BACK']}</a></p>";
			return;
		}
		echo "<form action=\"$uri\" method=\"post\">\n";
		echo '<input type="hidden" name="demo" value="0"/>';
		$str=($tries[0]==1)? $i18['ONE_MORE_COPY']."<br/><br/>": '';
		echo dload_tries_prompt($tries, 0, $str, $i18['GENERATE?']);
		echo "</form>";
	}
	echo "<br/><br/><br/>\n";
	echo "<form action=\"$uri\" method=\"post\">\n";
    echo '<input type="hidden" name="demo" value="1"/>';
	echo "<h4>".sprintf($i18['DLOAD4YEAR'],"<select name=\"yagree\">$out</select>")."</h4>";
	echo dload_tries_prompt($tries, 2, '', $i18['GENERATE?']);
	echo "</form>";
    return;
}

include_once("mobi/ipblock.php");
//$msg=allow_ip('buy', false);
echo $msg;
#if($msg) return;

$agree=dload_prompt(sprintf($i18['CONFIRM_TRIAL'], $lang, $lang), false);
$sess=session_name().'='.session_id();

// TODO: maintain these options when changing dic_paymode table!

$link['02'] = "http://$lang.wikipedia.com/wiki/PayPal";
$langwiki = strcmp($lang, 'ru') == 0 ? '' : "$lang:";
$link['04'] = "/wiki/doku.php/{$langwiki}bill";
$link['06'] = 'http://home.plimus.com/ecommerce/';
$link['07'] = 'http://www.shareit.com/';

$out='';
foreach($GLOBALS['amax']['paymodes'] as $key){
    $key2=sprintf('%02d', $key);
    $out.='<li>'.$i18['PAYMENT_'.$key2].
		' <a href="'.$link[$key2].'" target="_blank"><img src="/i/info.png" width="12" height="12" alt="?"/></a><br/><br/></li>';
}
$desc=sprintf($i18['REGFORM_DESC'], "<ul>$out</ul>");
echo <<< EOF
$desc
<form id="regform" action="$uri" method="post">
EOF;
/*
<p>{$i18['REGFORM_PAYMODE']}</p>
<p class="no_border">
foreach($GLOBALS['amax']['paymodes'] as $key){
    $key2=sprintf('%02d', $key);
    echo paymode_radio($i18['PAYMENT_'.$key2], 2, $key2, true);
}
*/
echo <<< EOF
</p>
<input type="hidden" name="demo"/>
<p>{$i18['REGFORM_EMAIL_1']}<br/>
<input type="text" name="email1" size="25" value="$email"/><br/>
{$i18['REGFORM_EMAIL_2']}<br/>
<input type="text" name="email2" size="25" value="$email2"/></p>
<p>{$i18['REGFORM_NAME']}<br/>
<input type="text" name="nick" size="25" value="$nick"/></p>
<p><img src="/mobi/kcaptcha?$sess" alt="Captcha"/>
<input type="text" name="p_captcha" size="8"/></p>
$agree
</form>
EOF;

return;

?>
<?php include_once('paypal.php') ?>
<?php include_once('bank.php') ?>
<!--
<p>Пожалуйста, заполните форму регистрации:
<form id="regform" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">
<p><input type="text" name="fullname"> Полное имя*</p>
<p><input type="text" name="email"> Email* </p>
<input type="submit" name="reg_submit" value="Зарегистрироваться">
</form>
-->
<?php

function dload_tries_prompt($arr, $key, $str, $prompt){
	global $DLIM;
	$disabled=false;
	$num=$arr[$key];
	$rem='';
	if($num!=-1){
		$rem=tries_remained($num, $DLIM[$key]);
	}
	if(!$num){
		$num='<span class="alert">'.$num."</span>";
		$disabled=true;
	}
	return $rem."<br/>".$str.dload_prompt($prompt, $disabled);
}

function paymode_radio($text, $paymode_id, $popup_id, $is_selected){
    global $lang_;
    $chk = $is_selected ? ' checked="checked"' : '';
    return <<< EOF
<input type="radio" name="paymode" value="{$paymode_id}"{$chk}/> {$text}&nbsp;
<a href="/popup.php?{$lang_}&amp;n={$popup_id}" target="_blank"><u>?</u></a><br/>
EOF;
}
?>
