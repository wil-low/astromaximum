<h4>Загрузка демо-версии</h4>
<p>
<?php
include_once("mobi/dbconnect.php"); 
include_once("mobi/amtools.php");
if(check_access()!=-1){
	if(isset($_POST["agree"])){
		$sc=get_default_cities(); 
		$current_year=get_year();
		echo midlet_create("demo", $current_year, $lang, $sc, "mobi/dl");
		return;
	}
	dload_prompt("Сгенерировать демо-версию?");
}
else
{
?>
Демо-версия календаря <b>ASTROMAXIMUM</b> содержит астрологические события прошлого года.</p>
<p>Если Вы - незарегистрированный пользователь, для загрузки демо-версии введите в форме слева:<br/>
<ul>
<li>логин: 1234</li>
<li>пароль: 5678</li>
</ul>
и нажмите "вход".</p>
<?php } 
?>