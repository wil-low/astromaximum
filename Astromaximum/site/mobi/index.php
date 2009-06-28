<?php 
$EXEC=2;
$lang='en';
header ("Content-Type: application/xhtml+xml");
header ("Cache-Control: max-age=3600");
$custom_content=''; $subtitle=''; $onload=''; $head='';

function output_callback($buffer)
{
	global $subtitle, $onload, $head;
	$buffer=str_replace("[[nav]]", implode(' ', $_SESSION['nav']), $buffer);
    if($subtitle)
        $subtitle='<span class="hdr">'.$subtitle.'</span><br/>';
	$buffer=str_replace("[[subtitle]]", $subtitle, $buffer);
    if($onload)
        $onload=' onload="'.$onload.'"';
	$buffer=str_replace("[[onload]]", $onload, $buffer);
	$buffer=str_replace("[[head]]", $head, $buffer);
	return $buffer;
}

ob_start("output_callback");

include_once('config.php');
include_once('lang.php');
include_once('dbconnect.php');
include_once('amtools.php');
$main='home';
if(isset($_GET['p'])){
	$main=$_GET['p'];
}
sess_start();
$sess=session_name().'='.session_id();

if(!isset($_SESSION['nav']) || !is_array($_SESSION['nav']))
    $_SESSION['nav'][0]="<a href=\"?$lang_&amp;p=selector&amp;$sess\">home</a>";
   
lang_load("html");
list($chac, $chac_pay)=check_access();
$user_ok=($chac>=0 and $chac!=1);

if(!preg_match("/^[\w_\d]+$/is", $main)){
	$main='home';
}
if(!preg_match("/^[_\d]+$/is", $main)){
	$main="m_$main";
}
$dir=dirname($_SERVER['SCRIPT_FILENAME']);
$fn="$dir/html/$lang/$main.php";
if(!file_exists($fn)){
	$fn="$dir/html/$main.php";
	if(!file_exists($fn)){
		$main='home';
		$fn="$dir/html/$lang/m_home.php";
	}
}
if(preg_match("/^(demo)$/is", $main)){
//	$show_topics=0;
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>mobi.astromaximum</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
[[head]]
</head>
<body[[onload]]>
<div id="hdr" class="nav"><p>[[nav]]</p></div>
<div id="cont">[[subtitle]]
<?php
	if($custom_content){
		echo $custom_content;
	}
	else{
		if(file_exists($fn)){
			$manual_requested=preg_match("/^[_\d]+$/is", $main);
			if($manual_requested){
#				if($user_ok){  # manuals are browsed for free
					echo str_replace('src="mobi/', 'src="', file_get_contents($fn));
					if(strcmp($main, '0_0')){
						echo "<p><br/><a href=\"?$lang_&amp;p=0_".$main{0}."\"><strong>{$i18['BACK_TOPIC']}</strong></a></p>";
					}
#				}
			}
			else{
				include($fn);
			}
		}
		else{
			echo "<h3>{$i18['PAGE_NOT_FOUND']}</h3>";
		}
	} 
?>
</div>
<div id="ftr"><p>
<?php
if($chac!=-1){
	echo $_SESSION['username'];
	echo " &nbsp; <a href=\"dl/logout.php\">logout</a>";
}
else{
	echo '* for demo:<br/> login: '.$GLOBALS['amax']['demo_login'].
		'<br/> pass: '.$GLOBALS['amax']['demo_pass'];
}
?>
</p></div>
</body></html>

<?php
ob_end_flush();

function addNavItem($linkparam, $title, $crop){
    global $lang_, $sess;
    if($crop>=0)
        $_SESSION['nav']=array_slice($_SESSION['nav'], 0, $crop);
    if(!strlen($linkparam))
    	$item = $title;
    else
        $item = "<a href=\"?$lang_&amp;p=$linkparam&amp;$sess\">$title</a>";
    $_SESSION['nav'][$crop-1]=$item;
}
?>
