<?php 
if(!isset($EXEC)) die("Access restricted");
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
	$tries=get_try_count(0);
	global $DLIM;
	$dl_key=0;
	$is_allow_dl=($tries[0]!=0);
	if(isset($_POST["yagree"])){ // past year request
		$year=(int)$_POST["yagree"];
		if($year>=$current_year){
			return;
		}
		$is_allow_dl=($tries[2]!=0);
		$dl_key=2;
	}
	echo "<h4>Загрузка календаря <b>ASTROMAXIMUM</b> на $year год</h4>";
	if(isset($_POST["agree"])){
		if($is_allow_dl){
			$sc=get_default_cities($_GLOBALS['amax']['def_cities']); 
			$str=midlet_create("tb", $year, $lang, $sc, "mobi/dl");
			if(strlen($str)){
				dec_try_count(0, $dl_key);
				echo $str;
				echo tries_remained($tries[$dl_key]-1, $DLIM[$dl_key]);
			}
		}
		else{
			echo 'Вам не разрешено загружать календарь. Обратитесь в <a href="#">службу поддержки</a>.';
		}
		return;
	}
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	echo "<form action=\"$uri\" method=\"post\">\n";
	$str="Я подтверждаю, что установил на свой телефон и успешно запустил ".
		"<a href=\"?lang=$lang&amp;p=demo\">демо-версию</a> календаря";
	echo dload_tries_prompt($tries, 0, $str);
	echo "</form>";
	echo "<br/><br/><br/>\n";
	echo "<form action=\"$uri\" method=\"post\">\n";
	echo "<h4>Загрузка календаря <b>ASTROMAXIMUM</b> на <select name=\"yagree\">$out</select> год</h4>";
	echo dload_tries_prompt($tries, 2, "Сгенерировать?");
	echo "</form>";
/*	}
	else{
		echo 'Вам не разрешено загружать календарь. Обратитесь в <a href="#">службу поддержки</a>.';
	}
*/
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

function dload_tries_prompt($arr, $key, $str){
	global $DLIM;
	$disabled=false;
	$num=$arr[$key];
	if(!$num){
		$num='<font color="red">'.$num."</font>";
		$disabled=true;
	}
	if($num!=-1){
		$str.=tries_remained($num, $DLIM[$key]);
	}
	return dload_prompt($str, $disabled);
}
?>