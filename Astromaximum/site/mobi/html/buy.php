<?php 
if(!isset($EXEC)) die("Access restricted");
include_once("mobi/amtools.php");
$current_year=get_year();
if(isset($_POST["reg_submit"])){ 
	echo $i18['INSTR_SENT'];
	return;
}
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
	echo "<h4>".sprintf($i18['DLOAD4YEAR'], $year)."</h4>";
	if(isset($_POST["agree"])){
		if($is_allow_dl){
			$sc=get_default_cities($GLOBALS['amax']['def_cities']); 
			$str=midlet_create("tb", $year, $lang, $sc, "mobi/dl", true);
			if(strlen($str)){
				dec_try_count(0, $dl_key);
				echo $str;
				echo tries_remained($tries[$dl_key]-1, $DLIM[$dl_key]);
			}
		}
		else{
			echo $i18['NO_CALENDAR_DL'];
		}
		return;
	}
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	echo "<form action=\"$uri\" method=\"post\">\n";
	$prompt=sprintf($i18['CONFIRM_TRIAL'], $lang_);
	$str=($tries[0]==1)? $i18['ONE_MORE_COPY']."<br/><br/>": '';
	echo dload_tries_prompt($tries, 0, $str, $prompt);
	echo "</form>";
	echo "<br/><br/><br/>\n";
	echo "<form action=\"$uri\" method=\"post\">\n";
	echo "<h4>".sprintf($i18['DLOAD4YEAR'],"<select name=\"yagree\">$out</select>")."</h4>";
	echo dload_tries_prompt($tries, 2, '', $i18['GENERATE?']);
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
<form id="regform" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">
<p><input type="text" name="fullname"> Полное имя*</p>
<p><input type="text" name="email"> Email* </p>
<input type="submit" name="reg_submit" value="Зарегистрироваться">  
</form>
-->
<?php } 

function dload_tries_prompt($arr, $key, $str, $prompt){
	global $DLIM;
	$disabled=false;
	$num=$arr[$key];
	$rem='';
	if($num!=-1){
		$rem=tries_remained($num, $DLIM[$key]);
	}
	if(!$num){
		$num='<font color="red">'.$num."</font>";
		$disabled=true;
	}
	return $rem."<br/><br/>".$str.dload_prompt($prompt, $disabled);
}
?>