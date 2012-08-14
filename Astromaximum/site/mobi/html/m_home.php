<?php
if(!isset($EXEC)) die("Access restricted");
$BIG_SITE="http://astromaximum.com/";
$MOBI_SITE="http://mobi.astromaximum.com/";
$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
if(!strpos($data_php, "mobi")){
	$data_php.="/mobi";
}
if($chac==-1){
	$login=''; $pass='';
	$is_entered = 0;
	if(isset($_POST['login'])){
		$login=$_POST['login'];
		$is_entered = 1;
	}
	if(isset($_POST['pass'])){
		$pass=$_POST['pass'];
		$is_entered = 1;
	}
	if($is_entered && !login($login, $pass)){
//		echo "Warning: logged as guest";
	}
	list($chac, $chac_pay)=check_access();
}

addNavItem('home', 'home', 0);

$validuser=false;
if($chac>=0 && $chac!=1){
	$validuser=true;
}
$current_year=$GLOBALS['amax']['year'];
if(isset($_POST['btn'])){
	$dest='ph';
	$btn=$_POST['btn'];
	exit;
}
$calendar_array = array();
$cities_caption = '';
$calendar_caption = '';
if ($validuser) {
	$cities_caption = $i18['SEL_DCITY'];
	$calendar_caption = "{$i18['SEL_DCALENDAR']}:<br/>";
	$year = $current_year + 1;
	while ($year >= $GLOBALS['amax']['min_demo_year']) {
		if (file_exists ("dl/source/$year.comm")) {
			$calendar_array[] = "<a href=\"?$lang_&amp;p=prg&amp;y=$year&amp;$sess\">$year</a> \n";
		}
		--$year;
	}
}
else {
	--$current_year;
	$cities_caption = $i18['SEL_DCITY'].' '.$current_year;
	$calendar_array[] = "<a href=\"?$lang_&amp;p=prg&amp;y=$current_year&amp;$sess\">".
		"{$i18['SEL_DCALENDAR']} $current_year</a> \n";
}
?>
<p>
<?php 
echo "<a href=\"?$lang_&amp;p=year&amp;$sess\">$cities_caption</a></p><p>$calendar_caption";
foreach ($calendar_array as $year_link) {
	echo $year_link;
}
?>
</p>
<?php
if (!$validuser) {
?>
<p><?php echo sprintf($i18['MOBI_CLIENT_LOGIN_H'], $GLOBALS['amax']['year']).':' ?></p>
<form action="<?php echo "?$lang_&amp;p=home" ?>" method="post">
<p>
login <input name="login" type="text" size="15" class="numinput" inputmode="digits"/><br/>
pass <input name="pass" type="password" size="15" class="numinput" inputmode="digits"/><br/>
<input type="submit" accesskey="1" name="action" value="Log in"/>
</p>
</form>
<?php
}
?>
