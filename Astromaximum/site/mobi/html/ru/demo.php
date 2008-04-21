<?php
if(!isset($EXEC)) die("Access restricted");
include_once("mobi/amtools.php");
include_once("mobi/ipblock.php");
allow_ip('demo',false);
if(check_access()!=-1){
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	echo "<h4>Загрузка демо-версии</h4><p>Демо-версия календаря <b>ASTROMAXIMUM</b> содержит астрологические события прошлого года.</p>";
	if(isset($_POST["agree"]) && isset($_POST['p_captcha'])){
		$captcha=$_POST['p_captcha'];
		if(is_capcha($captcha)){
			$choice=$_POST["agree"];
			$current_year=get_year()-1;
			if(strcmp($choice,"demo")==0){
				$sc=get_default_cities($GLOBALS['amax']['def_cities']); 
				echo midlet_create("demo", $current_year, $lang, $sc, "mobi/dl", true);
			}
			else{
				if(is_numeric($choice) && ($choice>=0) && ($choice<count($GLOBALS['amax']['demo_cities']))){
					$sc=get_default_cities($GLOBALS['amax']['demo_cities'][$choice]); 
					echo midlet_create("geo", $current_year, $lang, $sc, "mobi/dl", true);
				}
			}
			echo "<p><a href=\"$uri\">{$i18['BACK']}</a></p>";
			return;
		}
		echo "<p><font color=\"red\">Invalid string entered</font></p>";
	}
	$sess=session_name().'='.session_id();
	echo <<<EOF1
<form action="$uri" method="post">
<p>Введите символы, указанные на рисунке:</p>
<p><img src="mobi/kcaptcha?$sess">
<input name="p_captcha" type="text"/>
</p>
<p><font color="green">Сгенерировать календарь:</font><br/><br/>
<input type="radio" name="agree" value="demo" style="width:auto; border: 0px" checked="checked"/> <b>ASTROMAXIMUM</b> демо<br/><br/>
<p>После установки календаря вы можете загрузить любой из демо городов</p><br/>
<font color="green">Сгенерировать город:</font><br/><br/>
EOF1;
	foreach($GLOBALS['amax']['demo_cities'] as $i=>$city){
		echo '<input type="radio" name="agree" value="'.$i.'" style="width:auto; border: 0px"/> '.$city.' <br/><br/>';
	}
	echo '<input type="button" style="height:auto;" value="OK" onclick="checkCheckBox(this)"/></form>';
	return;
}
?>
<p>Если Вы - незарегистрированный пользователь, для загрузки демо-версии введите в форме слева:</p>
<ul>
<li>логин: 123456789</li>
<li>пароль: 012345678</li>
</ul>
<p>и нажмите "вход".</p>
