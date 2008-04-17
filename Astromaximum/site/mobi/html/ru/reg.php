<?php
if(!isset($EXEC)) die("Access restricted");
echo "<h4>Загрузка полной версии</h4><p>";
include_once("mobi/dbconnect.php");
include_once("mobi/amtools.php");
$current_year=get_year();
$chac=check_access(); 
if($chac!=-1 and $chac!=1){
	if(isset($_REQUEST["go"])){
		$sc=get_default_cities(); 
		echo midlet_create("demo", $current_year-1, $lang, $sc, "mobi/dl");
		return;
	}
?>
Нажмите на кнопку, чтобы загрузить демо-версию 
астрологического календаря <b>ASTROMAXIMUM</b>.</p><br/>
<form action="<?php echo $_SERVER['REQUEST_URI'] ?>&go=1" method="post">
<input type="submit" value="Демо">
</form>
<?php
}
else
{
?>
Дистрибутив календаря <b>ASTROMAXIMUM</b> уже содержит следующие города:
<ol><li>
<?php
	echo implode("</li><li>", $_GLOBALS['amax']['def_cities']);
?>
</li></ol>
Полная версия астрологического календаря доступна только зарегистрированным
пользователям.<br/>Введите свой логин и пароль в форме слева.</p>
<p>Если Вы - незарегистрированный пользователь, Вам следует оплатить стоимость календаря:</p>
<p><em><strong>&euro;<?php echo $price ?></strong> - полная версия на <?php echo $current_year ?> год</em></p>
одним из следующих способов:
<ul>
<li>почтовым переводом (<?php anchor("sendmoney") ?>наши реквизиты</a>)</li>
<li>через платежную систему <a href="http://paypal.com">PayPal</a></li>
</ul>
и нажмите "вход".</p>
<?php } 
?>