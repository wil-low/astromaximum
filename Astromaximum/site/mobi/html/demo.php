<?php
if(!isset($EXEC)) die("Access restricted");
include_once("mobi/amtools.php");

$uri=htmlentities($_SERVER['REQUEST_URI']);
$prev_year=$GLOBALS['amax']['year']-1;
if(isset($_POST['p_captcha'])){
	$captcha=$_POST['p_captcha'];
	if(is_captcha($captcha)){
		$sc=get_default_cities($GLOBALS['amax']['def_cities'][$lang]); 
		$link_text=midlet_create("demo", $prev_year, $lang, $sc, "mobi/dl", 2);
		if(!$link_text){
			echo '<p><span class="alert">Error</span></p>';
		}
		else{
			echo "<p>".$link_text."</p>";
			return;
		}
	}
	else{
		echo "<p><span class=\"alert\">{$i18['CAPTCHA_WRONG']}</span></p>";
	}
}
$sess=session_name().'='.session_id();
$msg_dlcity=sprintf($i18['DEMO_DLCITY'], $prev_year);
echo <<<EOF1

<form action="demo" method="post">
<p>{$i18['CAPTCHA_PROMPT']}</p>
<p><img src="/mobi/kcaptcha?$sess" alt="Captcha">
<input name="p_captcha" type="text" size="8"/>
</p>
<p><span class="fine">{$i18['DEMO_CALGEN']}:</span><br/><br/>
<!--<input type="radio" name="agree" value="demo" style="width:auto; border: 0px" checked="checked"/> -->
<b>ASTROMAXIMUM {$prev_year}</b><br/><br/>
<input type="button" class="ok_on" value="OK" onclick="form.submit()"/>
<br/><br/></p>
<p></p>
<p>{$msg_dlcity}</p>

EOF1;
?>
