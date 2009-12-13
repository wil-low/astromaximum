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
	if(isset($_POST['login'])){
		$login=$_POST['login'];
	}
	if(isset($_POST['pass'])){
		$pass=$_POST['pass'];
	}
	if(!login($login, $pass)){
	/*
		include_once("ipblock.php");
		$msg=allow_ip("mobi_home", true);
		echo $msg;
		if(!$msg) redirect($MOBI_SITE);
		exit;
		*/
	}
	list($chac, $chac_pay)=check_access();
}
$DEST_URL=array("year", "prg&amp;mode=demo", "prg&amp;mode=trial");

addNavItem('selector', 'home', 0);

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
$city_title = $i18['SEL_DCITY'];
if (!$validuser)
	$city_title .= ' ' . ($current_year-1)
?>

<form action="<?php echo htmlentities($_SERVER['REQUEST_URI']) ?>" method="post"><p>
<?php echo knopka(1, true, $city_title) ?><br/>
<?php echo knopka(2, true, 'Astromaximum '.($current_year-1)) ?>*<br/>
<?php echo knopka(3, $validuser, 'Astromaximum '.$current_year) ?>*</p>
</form>
<p>* <?php echo "{$i18['SEL_CHK1']} <b>".gmdate("M d Y") ?></b>
<br/><br/>
<span id="phdate">Please check phone's date</span>
</p>
<?php

$head = <<< EOF
<script type="text/javascript">
<!--
function curDate(){
    var date=new Date();
    var s=date.toString();
    s=s.substr(4, 11);
    document.getElementById("phdate").innerHTML='Phone date: <b>'+s+'</b>';
}
//-->
</script>
EOF;
$onload='curDate()';

function knopka($key, $is_valid, $text){
  global $DEST_URL, $sess, $lang_;
//  $str="<input type=\"submit\" accesskey=\"$key\" name=\"btn\" value=\"$key\" style=\"width:2em;\"$disa/> ";
  if($is_valid){
	 $text="<a href=\"?$lang_&amp;p={$DEST_URL[$key-1]}&amp;$sess\">$text</a>";
  }
  return "$key. $text";
}
?>