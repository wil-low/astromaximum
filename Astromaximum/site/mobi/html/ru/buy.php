<?php 
	include_once("mobi/amtools.php"); 
	$current_year=get_year();
?>
<h4>Загрузка полной версии на <?php echo $current_year ?> год</h4>
<?php
if(isset($_POST["reg_submit"])){ ?>
	Благодарим Вас. На указанный адрес отправлено письмо с инструкциями по покупке
	астрологического календаря.<br/>После оплаты мы вышлем Вам логин и пароль для доступа
	к интерфейсу загрузки.
<?php
	return;
}
?>
<p>
Дистрибутив календаря <b>ASTROMAXIMUM</b> уже содержит следующие города:
<?php
echo "<ol><li>".implode("</li><li>", $DEF_CITIES)."</li></ol>";
include_once("mobi/dbconnect.php");
$chac=check_access(); 
if($chac!=-1 and $chac!=1){
	if(isset($_POST["agree"])){
		$sc=get_default_cities(); 
		echo midlet_create("tb", $current_year, $lang, $sc, "mobi/dl");
		return;
	}
	dload_prompt("Я подтверждаю, что установил на свой телефон и успешно запустил ".
		"<a href=\"?lang=$lang&p=demo\">демо-версию</a> календаря");
}
else
{
echo reg_warning("Полная версия астрологического календаря");
?>
</p><br/><br/>
<p>Если Вы - незарегистрированный пользователь,<br/>Вам следует
оплатить стоимость календаря.</p>
<p>Пожалуйста, заполните форму регистрации:
<form name="regform" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">
<p><input type="text" name="fullname"> Полное имя*</p>
<p><input type="text" name="email"> Email* </p>
<input type="submit" name="reg_submit" value="Зарегистрироваться">  
</form></p>
<?php } 
?>