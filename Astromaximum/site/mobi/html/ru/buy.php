<?php 
	include_once("mobi/amtools.php");
	$current_year=get_year();
?>
<?php
if(isset($_POST["reg_submit"])){ ?>
	Благодарим Вас. На указанный адрес отправлено письмо с инструкциями по покупке
	астрологического календаря.<br/>После оплаты мы вышлем Вам логин и пароль для доступа
	к интерфейсу загрузки.
<?php
	return;
}
include_once("mobi/dbconnect.php");
$chac=check_access(); 
if($chac!=-1 and $chac!=1){
	$y_now=date("Y");
	$out='';
	for($i=1; $i<3; $i++){
		$yy=$y_now-$i;
		$out.="<option value=\"$yy\"";
		if($i==1) $out.=" selected=\"selected\"";
		$out.=">$yy</option>\n";
	}
	$year=$current_year;
	if(isset($_POST["yagree"])){
		$year=(int)$_POST["yagree"];
		if($year>=$current_year){
			return;
		}
	}
	echo "<h4>Загрузка календаря <b>ASTROMAXIMUM</b> на $year год</h4>";
	if(isset($_POST["agree"])){
		global $DEF_CITIES;
		$sc=get_default_cities($DEF_CITIES); 
		echo midlet_create("tb", $year, $lang, $sc, "mobi/dl");
		return;
	}
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	echo "<form action=\"$uri\" method=\"post\">\n";
	dload_prompt("Я подтверждаю, что установил на свой телефон и успешно запустил ".
		"<a href=\"?lang=$lang&amp;p=demo\">демо-версию</a> календаря");
	echo "</form>";
	echo "<br/><br/><br/>\n";
	echo "<form action=\"$uri\" method=\"post\">\n";
	echo "<h4>Загрузка календаря <b>ASTROMAXIMUM</b> на <select name=\"yagree\">$out</select> год</h4>";
	dload_prompt("Сгенерировать?");
	echo "</form>";
}
else
{
echo "<h4>Выберите вид оплаты:</h4>";
?>
<ul>
<li><a href="#">PayPal</a></li>
<li><a href="#">Другие</a></li>
</ul>
<!--
<p>Пожалуйста, заполните форму регистрации:
<form name="regform" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">
<p><input type="text" name="fullname"> Полное имя*</p>
<p><input type="text" name="email"> Email* </p>
<input type="submit" name="reg_submit" value="Зарегистрироваться">  
</form>
-->
<?php } 
?>