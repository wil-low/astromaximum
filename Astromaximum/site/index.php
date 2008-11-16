<?php 
$EXEC=1;

$META_KEYWORDS=''; $META_DESCR=''; $META_TITLE='';
$META_CUSTOMSCR=''; $META_CUSTOMFUNC=''; $META_HEAD_ADD='';
function output_callback($buffer)
{
	global $META_TITLE, $META_KEYWORDS, $META_DESCR,
        $META_CUSTOMSCR, $META_CUSTOMFUNC,$META_HEAD_ADD;
	// fill meta tags
	if($META_TITLE){ 
		$META_TITLE.=" - ";
	}
	$META_TITLE.="ASTROMAXIMUM"; 
	$buffer=str_replace("[[title]]", $META_TITLE, $buffer);
	$buffer=str_replace("[[keywords]]", $META_KEYWORDS, $buffer);
	$buffer=str_replace("[[description]]", $META_DESCR, $buffer);
    if($META_CUSTOMSCR)
		$META_CUSTOMSCR='<script src="'.$META_CUSTOMSCR.'" type="text/javascript"></script>';
	$buffer=str_replace("[[onload_script]]", $META_CUSTOMSCR, $buffer);
	if($META_CUSTOMFUNC)
		$META_CUSTOMFUNC=' onload="'.$META_CUSTOMFUNC.'"';
	$buffer=str_replace("[[onload_func]]", $META_CUSTOMFUNC, $buffer);
	$buffer=str_replace("[[head_add]]", $META_HEAD_ADD, $buffer);
	return $buffer;
}

ob_start("output_callback");

include_once('mobi/amtools.php');
$main='home';
if(isset($_GET['p'])){
	$main=$_GET['p'];
}
sess_start();
include_once('mobi/config.php');
include_once('mobi/lang.php');
include_once('mobi/dbconnect.php');
lang_load("mobi/html");
list($chac, $chac_pay)=check_access();
$user_ok=($chac>=0 and $chac!=1);
$show_topics=1;
$custom_content='';

if(strcmp($main, 'login')==0){
	$login=''; $pass='';
	if(isset($_POST['login'])){
		$login=$_POST['login'];
	}
	if(isset($_POST['pass'])){
		$pass=$_POST['pass'];
	}
	if(login($login, $pass)){
		$main='home';
		if(isset($_GET['to']) && strcmp($_GET['to'],'demo')==0){
			$main=$_GET['to'];
		}
	}
	else{
		include_once("mobi/ipblock.php");
		$custom_content=allow_ip('login', false);
		$main='home';
	}
	if(!$custom_content){
		redirect("?$lang_&amp;p=$main");
	}	
}
if(!preg_match("/^[\w_\d]+$/is", $main)){
	$main='home';
}
$dir=dirname($_SERVER['SCRIPT_FILENAME']);
$fn="$dir/mobi/html/$lang/$main.php";
if(!file_exists($fn)){
	$fn="$dir/mobi/html/$main.php";
	if(!file_exists($fn)){
		$main='home';
		$fn="$dir/mobi/html/$lang/home.php";
	}
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>[[title]]</title>
<meta name="author" content="Willow"/>
<meta name="generator" content="Bluefish 1.0.7"/>
<meta name="copyright" content="Copyright (c) by S&amp;W Axis"/>
<meta name="keywords" content="[[keywords]]"/>
<meta name="description" content="[[description]]"/>
<link href="astro.css" rel="stylesheet" type="text/css"/>
<script src="./func.js" type="text/javascript"></script>

<script type="text/javascript">
<!--
if (document.images){
    preload_image_object = new Image();
    // set image url
    image_url = new Array();
    image_url[0] = "i/globe.jpg";
    image_url[1] = "i/fon.jpg";
    image_url[2] = "i/button.jpg";
    
    var i = 0;
    for(i=0; i<=2; i++) 
        preload_image_object.src = image_url[i];
}
//-->
</script>     
  
[[onload_script]]
[[head_add]]
</head>
<body[[onload_func]]>
<a id="top"></a>
<div id="globe">
<img src="i/globe.jpg" width="956" height="320" usemap="#Map" alt="ASTROMAXIMUM"/>
<map id="Map" name="Map">
	<area shape="circle" coords="178,132,95" href="?<?php echo $lang_ ?>" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" />
	<area shape="rect" coords="443,87,859,165" href="?<?php echo $lang_ ?>" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" />
</map>
</div>
<div id="logoText"><?php echo $i18['AMAX_LOGO'] ?></div>
<noscript>
	<div id="js_warn" class="alert"><?php echo $i18['ENABLE_JS']?></div>
</noscript>
<div id="lang">
<?php
	$lng=array('DE', 'EN', 'RU');
	for($i=0; $i<count($lng); $i++){
		if($i) echo " | ";
		$lng2=strtolower($lng[$i]);
		if(strcmp($lang, $lng2)==0){
			echo "<b>$lng[$i]</b>";
		}
		else{
			echo "<a href=\"?lang=$lng2&amp;p=$main\">$lng[$i]</a>";
		}
	}
?>
<br />
<p><b>GMT <?php echo gmstrftime("%H:%M") ?></b></p>
</div>
<div id="menu">
<a href="?<?php echo "$lang_\">".$i18['MNU_HOME']?></a> | 
<?php echo anchor('man0').$i18['MNU_MAN']?></a> | 
<?php echo anchor('scr').$i18['MNU_SCRSHOTS'] ?></a> | 
<?php echo anchor('buy').$i18['MNU_BUY'] ?></a> | 
<?php echo anchor('citylist').$i18['MNU_CITYLIST'] ?></a> |
<?php echo anchor('dl').$i18['MNU_DLCIT'] ?></a> | 
<a href="#"><?php echo $i18['MNU_CONTACTS']?></a>
<?php 
//echo "<br/>";print_r($_REQUEST);
$btn1=$i18['DEMO']."<br/>+ ".$i18['CITY_MODULE']; $btn1_link="demo";
$btn2=$i18['ORDER']." {$GLOBALS['amax']['price']}<br/>+ {$GLOBALS['amax']['city_count']} ".$i18['_CITIES'];
if($chac==0){
	echo <<<ADMIN_TB
	<p>| 
	<a href="?$lang_&amp;p=env">окружение</a> |  
	<a href="?$lang_&amp;p=db_stats">статистика</a> |  
	<a href="?$lang_&amp;p=upload">загрузка городов</a> |
	<a href="?$lang_&amp;p=usermgr">пользователи</a> |
	</p>
ADMIN_TB;
}
if($user_ok){
	if(strcmp($main, 'demo')){
		$btn1=$i18['CITY_BUTTON']; $btn1_link="dl";
	}
	if(strcmp($main, 'dl')){
		$btn1=$i18['CITY_BUTTON']; $btn1_link="dl";
	}
	$btn2=$i18['TRIAL'];
}
if($chac==1){
	$btn2=$i18['ORDER']." {$GLOBALS['amax']['price']}<br/>+ {$GLOBALS['amax']['city_count']} ".$i18['_CITIES'];
}
if($chac!=-1){
	$session_prompt=<<<SP1
<p>{$i18['WELCOME']}, <b>{$_SESSION['username']}</b> ! </p>
<p><a href="mobi/dl/logout.php"><strong>{$i18['LOGOUT']}</strong></a></p> 
SP1;
}
else{ 
	$session_prompt=<<<FRM
<form id="flog" action="?$lang_&amp;p=login&amp;to=$main" method="post"> 
<input id="ilog" name="login"/> e-mail <br /><br />
<input id="ipwd" name="pass" type="password"/> password <br /><br />
<input type="submit" class="loginbutton" onclick="return checklogin()" value="{$i18['LOG_IN']}" /> | 
<a class="loginbutton" href="?$lang_&amp;p=pwdrestore">{$i18['LOST_PWD']}</a>
</form> 
FRM;
} 
?> 
</div>
<?php 
//if($chac<0 || (strcmp($main, 'dl') && strcmp($main, 'dl2'))){ 
	echo disable_big_button('demo', $btn1, $btn1_link, $btn1_link);
	echo disable_big_button('buy', $btn2, 'buy', 'buy');
//}
?>
<div id="leftColumn">
<?php	echo $session_prompt ?>

<?php
if(preg_match("/^man\d$/is", $main)){
    echo "<h5>".$i18['MAN_TOPICS']."</h5>";
    for($i=0; $i<=4; $i++){
        echo "<p>".anchor("man$i")."<img src=\"i/ico.gif\" alt=\"\"/> <br/><b>".$i18["MAN_$i"]."</b></a></p>\n";
    }
    $pdf_path="mobi/html/amax-manual-$lang.pdf";
    echo "<p><a href=\"$pdf_path\"><img src=\"i/ico.gif\" alt=\"\"/> <br/><b>".$i18['MAN_PDF']." (PDF ".
        fsize_human($pdf_path).")</b></a></p>\n";
}
else{
    if($show_topics){
        echo "<h5>".$i18['THEMES_CAL']."</h5>";
        for($i=1; $i<=9; $i++){
            echo "<p>".anchor("0_$i")."<img src=\"i/ico.gif\" alt=\"\"/> <br/><b>".$i18["THEME_$i"]."</b></a></p>\n";
        }
    }
}
?>
</div><!-- end leftColumn div -->
<div id="content">
<?php
	if($custom_content){
		echo $custom_content;
	}
	else{
		if(file_exists($fn)){
			$topic_requested=preg_match('/^(.+\/)(\d+)_(\d+)\.php$/is', $fn, $matches);
			if($topic_requested){
                prepare_topic($matches, $main);
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

</div><!-- end content div -->
<div id="bottom"><p>Copyright &copy;
2007-2008
S&amp;W Axis. All rights reserved.   &nbsp;&nbsp;    <a href="http://goglus.com">design goglus</a></p></div>
</body>
</html>

<?php
ob_end_flush();

function disable_big_button($id, $label, $check_page, $link_page){
	global $main, $lang_;
	$style='';
	if(strcmp($main, $check_page)){
		$label="<a href=\"?$lang_&amp;p=$link_page\">".$label.'</a>';
	}
	else{
		$style=' style="color:rgb(133,195,224)"';
	}
	return "<div id=\"$id\"$style>$label</div>\n";
}

function prepare_topic($matches, $main){
    global $lang_, $i18;
    $nums=explode('_', $main);
    if($matches[2]+$matches[3]){
        if($matches[2]){
            $fn0=$matches[1].$matches[2].'_0.php';
        }
        else{
            $fn0=$matches[1].$matches[3].'_0.php';
            
        }
        if(file_exists($fn0)){
            include($fn0);
            if(strcmp($main, '0_0')){
                echo "<p><a href=\"?$lang_&amp;p=0_".$main{0}."\"><strong>{$i18['BACK_TOPIC']}</strong></a><br/></p>";
            }
        }
    }
    include($matches[0]);
}
?>
