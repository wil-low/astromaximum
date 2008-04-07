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
	
	$dl_key='dl';
	
	$stat=sprintf("SELECT dl_count, past_count FROM customers WHERE id=%d", quote_smart($_SESSION['uid']));
	$sth=mysql_query($stat);
	global $DLIM;
	if($sth && ($row=mysql_fetch_array($sth, MYSQL_BOTH))){
		$is_allow_dl=($row[0]!=0);
		if(isset($_POST["yagree"])){
			$year=(int)$_POST["yagree"];
			if($year>=$current_year){
				return;
			}
			$is_allow_dl=($row[1]!=0);
			$dl_key='past';
		}
		echo "<h4>Загрузка календаря <b>ASTROMAXIMUM</b> на $year год</h4>";
		if(isset($_POST["agree"])){
			global $DEF_CITIES;
			if($is_allow_dl){
				$sc=get_default_cities($DEF_CITIES); 
				$str=midlet_create("tb", $year, $lang, $sc, "mobi/dl");
				if(strlen($str)){
					$stat=sprintf("UPDATE customers SET $dl_key=$dl_key-1 WHERE id=%d", quote_smart($_SESSION['uid']));
					$sth=mysql_query($stat);
					echo $str;
					echo tries_remained($row[$dl_key]-1, $DLIM[$dl_key]);
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
		echo dload_tries_prompt($row, 'dl_count', $str);
		echo "</form>";
		echo "<br/><br/><br/>\n";
		echo "<form action=\"$uri\" method=\"post\">\n";
		echo "<h4>Загрузка календаря <b>ASTROMAXIMUM</b> на <select name=\"yagree\">$out</select> год</h4>";
		echo dload_tries_prompt($row, 'past_count', "Сгенерировать?");
		echo "</form>";
	}
	else{
		echo 'Вам не разрешено загружать календарь. Обратитесь в <a href="#">службу поддержки</a>.';
	}
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

function tries_remained($tries, $limit) {
	return "<br/><br/>Осталось попыток: <b>$tries из $limit</b>";
}

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