<?php
if(!isset($EXEC)) die("Access restricted");
include_once("mobi/amtools.php");
include_once("mobi/ipblock.php");
allow_ip('demo',false);
if(check_access()!=-1){
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	if(isset($_POST["agree"]) && isset($_POST['p_captcha'])){
		$captcha=$_POST['p_captcha'];
		if(is_captcha($captcha)){
			$choice=$_POST["agree"];
			$prev_year=$GLOBALS['amax']['year']-1;
			if(strcmp($choice,"demo")==0){
				$sc=get_default_cities($GLOBALS['amax']['def_cities']); 
				echo midlet_create("demo", $prev_year, $lang, $sc, "mobi/dl", true);
			}
			else{
				if(is_numeric($choice) && ($choice>=0) && ($choice<count($GLOBALS['amax']['demo_cities']))){
					echo "<p>".sprintf($i18['READY_CITIES'], $GLOBALS['amax']['demo_cities'][$choice], $prev_year)."</p>\n";				
					$sc=get_default_cities($GLOBALS['amax']['demo_cities'][$choice]); 
					echo midlet_create("geo", $prev_year, $lang, $sc, "mobi/dl", true);
				}
			}
			echo "<p><a href=\"$uri\">{$i18['BACK']}</a></p>";
			return;
		}
		echo "<p><font color=\"red\">{$i18['CAPTCHA_WRONG']}</font></p>";
	}
	$sess=session_name().'='.session_id();
	echo $i18['DEMO_HEADER'];
	echo <<<EOF1
<form action="$uri" method="post">
<p>{$i18['CAPTCHA_PROMPT']}</p>
<p><img src="mobi/kcaptcha?$sess">
<input name="p_captcha" type="text"/>
</p>
<p><font color="green">{$i18['DEMO_CALGEN']}:</font><br/><br/>
<input type="radio" name="agree" value="demo" style="width:auto; border: 0px" checked="checked"/> 
<b>ASTROMAXIMUM</b> {$i18['DEMO_DEMO']}<br/><br/>
<p>{$i18['DEMO_DLCITY']}</p><br/>
<font color="green">{$i18['SELCITY_GENERATE']}:</font><br/><br/>
EOF1;
	foreach($GLOBALS['amax']['demo_cities'] as $i=>$city){
		echo '<input type="radio" name="agree" value="'.$i.'" style="width:auto; border: 0px"/> '.$city.' <br/><br/>';
	}
	echo '<input type="button" class="ok_on" value="OK" onclick="checkCheckBox(this)"/></form>';
	return;
}
$press_enter=sprintf($i18['DEMO_ENTER'],$i18['LOG_IN']);
echo <<<EOF2
<p>{$i18['DEMO_NOTREG']}</p>
<ul>
<li>{$i18['LOGIN']}: {$GLOBALS['amax']['demo_login']}</li>
<li>{$i18['PWD']}: {$GLOBALS['amax']['demo_pass']}</li>
</ul>
<p>$press_enter</p>
EOF2;
?>