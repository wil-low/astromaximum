<?php
$BIG_SITE="http://astromaximum.com/";
$MOBI_SITE="http://mobi.astromaximum.com/";
$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
if(!strpos($data_php, "mobi")){
	$data_php.="/mobi";
}
$chac=check_access();
if($chac==-1){
	$login=''; $pass='';
	if(isset($_POST['login'])){
		$login=$_POST['login'];
	}
	if(isset($_POST['pass'])){
		$pass=$_POST['pass'];
	}
	if(!login($login, $pass)){
		include_once("ipblock.php");
		$msg=allow_ip("mobi_home", true);
		echo $msg;
		if(!$msg) redirect($MOBI_SITE);
		exit;
	}
}
$DEST_URL=array("year", "about", "prg&amp;mode=demo", "prg&amp;mode=trial");

addNavItem('selector', 'home', 0);

$chac=check_access();
$validuser=false;
if($chac>=0 && $chac!=1){
	$validuser=true;
}
$current_year=$GLOBALS['amax']['year'];
if(isset($_POST['btn'])){
	$dest='ph';
	$btn=$_POST['btn'];
/*
	if(strcmp($dest, 'pc')==0){ // PC links
		$desturl=$BIG_SITE."?lang=$lang";
		switch($btn){
			case 1:	$desturl.="&p=dl"; break;
			case 2:	unset($desturl); break;
//			case 3:	$desturl=$BIG_SITE."&p=dl"; break;
			case 4:	$desturl.="&p=demo"; break;
			case 5:	$desturl.="&p=buy"; break;
		}
	}
	else{ // Phone links
		switch($btn){
			case 1:	$desturl="year.php?"; break;
			case 2:	$desturl="index.php?lang=$lang&p=0_0"; break;
			case 3:	$desturl="html/$lang/about.xhtml?"; break;
			case 4:	$desturl="dl/prg.php?mode=demo&lang=$lang&dest=$dest"; break;
			case 5:	$desturl="democity.php?"; break;
			case 6:	$desturl="dl/prg.php?mode=trial&lang=$lang&dest=$dest"; break;
		}
	}
	if(isset($desturl)){
		$desturl.='&'.session_name().'='.session_id();
		header("Location: http://$data_php/$desturl");
	}
*/	
	exit;
}
?>
<form action="<?php echo htmlentities($_SERVER['REQUEST_URI']) ?>" method="post"><p>
<?php echo knopka(1, $chac>=0, $i18['SEL_DCITY']) ?><br/>
<?php echo knopka(2, true, $i18['SEL_ABOUT']) ?><br/>
<?php echo knopka(3, true, $i18['SEL_DEMO'].' '.($current_year-1)) ?>*<br/>
<?php echo knopka(4, $validuser, $current_year) ?>*</p>
</form>
<p>* <?php echo "{$i18['SEL_CHK1']} <b>".gmdate("M d Y") ?></b>
<br/>&nbsp;&nbsp;
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