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

include_once('mobi/config.php');
include_once('mobi/lang.php');
include_once('mobi/dbconnect.php');
include_once('mobi/amtools.php');

$main='home';
if(isset($_GET['p'])){
	$main=$_GET['p'];
}

detect_mobile();

ob_start("output_callback");

sess_start();
lang_load("mobi/html");
list($chac, $chac_pay)=check_access();
$user_ok=($chac>=0 and $chac!=1);
$show_topics=1;
$custom_content='';
$buy_page = 'p_07'; // 'buy'

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
		list($chac, $chac_pay)=check_access();
		if(isset($_GET['to']) && strcmp($_GET['to'],'demo')==0){
			$main=$_GET['to'];
		}
		if ($chac == 3) $main = $buy_page;
 	}
	else{
		include_once("mobi/ipblock.php");
		$main='home';
	}
	redirect("../$main");
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
<meta name="author" content="S&amp;W Axis"/>
<meta name="generator" content="Bluefish 1.0.7"/>
<meta name="copyright" content="Copyright (c) by S&amp;W Axis"/>
<meta name="keywords" content="[[keywords]]"/>
<meta name="description" content="[[description]]"/>
<meta name="verify-v1" content="A4EqTQ801cLBIuT4iqqotxVkCPj3AFEDhLvhN3xdMXs=" />
<link href="/astro.css" rel="stylesheet" type="text/css"/>
<script src="/func.js" type="text/javascript"></script>
<script type="text/javascript">
<!--
if (document.images){
    preload_image_object = new Image();
    // set image url
    image_url = new Array();
    image_url[0] = "/i/globe.jpg";
    image_url[1] = "/i/fon.jpg";
    image_url[2] = "/i/button.jpg";
    
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
<img src="/i/globe.jpg" width="956" height="320" usemap="#Map" alt="ASTROMAXIMUM"/>
<map id="Map" name="Map">
	<area shape="circle" coords="178,132,95" href="/<?php echo "$lang/home" ?>" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" />
	<area shape="rect" coords="443,87,859,165" href="/<?php echo "$lang/home" ?>" alt="ASTROMAXIMUM" title="ASTROMAXIMUM" />
</map>
</div>
<div id="logoText"><?php echo $i18['AMAX_LOGO'] ?></div>
<noscript>
	<div id="js_warn" class="alert"><?php echo $i18['ENABLE_JS']?></div>
</noscript>
<div id="lang">
<?php
// language selection
	$lng=array(/*'DE',*/ 'EN', 'RU');
	for($i=0; $i<count($lng); $i++){
		if($i) echo " | ";
		$lng2=strtolower($lng[$i]);
		if(strcmp($lang, $lng2)==0){
			echo "<b>$lng[$i]</b>";
		}
		else{
			echo "<a href=\"/$lng2/$main\">$lng[$i]</a>";
		}
	}
?>
<br />
<p><b>GMT <?php echo gmstrftime("%H:%M") ?></b></p>
</div>
<div id="menu">
<?php
print_menu('home', 'MNU_HOME', 1);
print_menu('man0', 'MNU_MAN', 1);
echo "<a href=\"/wiki/doku.php/" . (strcmp ($lang, 'ru') ? "$lang/" : "") . "start\">wiki</a> | ";
echo "<a href=\"/wiki/doku.php/" . (strcmp ($lang, 'ru') ? "$lang/" : "") . 
	"screen\" target=\"_blank\">{$i18['MNU_SCRSHOTS']}</a> | ";
print_menu($buy_page, 'MNU_BUY', 1);
print_menu('citylist', 'MNU_CITYLIST', 1);
print_menu('dl', 'MNU_DLCIT', 1);
print_menu('contacts', 'MNU_CONTACTS', 0);
//echo "<br/>";print_r($_REQUEST);
$btn1=sprintf($i18['DEMO'], $GLOBALS['amax']['year'] - 1); $btn1_link="/$lang/demo";
$btn2=sprintf($i18['ORDER'], $GLOBALS['amax']['year']);
if($chac==0){
	echo <<<ADMIN_TB
	<p>| 
	<a href="/$lang/env">окружение</a> |  
	<a href="/$lang/db_stats">статистика</a> |  
	<a href="/$lang/upload">загрузка городов</a> |
	<a href="/$lang/usermgr">пользователи</a> |
	</p>
ADMIN_TB;
}
if($user_ok){
	if(strcmp($main, 'demo')){
		$btn1=$i18['CITY_BUTTON']; $btn1_link="dl";
	}
	if(strcmp($main, 'dl') && strcmp($main, 'dl2')){
		$btn1=$i18['CITY_BUTTON']; $btn1_link="dl";
	}
	$try_count = get_try_count(0);
	if ($try_count[0] != 0)
		$btn2=$i18['TRIAL'];
}
/*
if($chac==1 || $chac==3){
	$btn2=sprintf($i18['ORDER'], $GLOBALS['amax']['price'])."<br/> + {$GLOBALS['amax']['city_count']} ".
	$i18['_CITIES'];
}
*/
if($chac!=-1){
	$session_prompt=<<<SP1
<p>{$i18['WELCOME']}, <b>{$_SESSION['username']}</b> ! </p>
<p><a href="/mobi/dl/logout.php"><strong>{$i18['LOGOUT']}</strong></a></p> 
SP1;
}
else{ 
	$session_prompt=<<<FRM
<form id="flog" action="/$lang/login/to=$main" method="post"> 
<input id="ilog" name="login"/> e-mail <br /><br />
<input id="ipwd" name="pass" type="password"/> password <br /><br />
<input type="submit" class="loginbutton" onclick="return checklogin()" value="{$i18['LOG_IN']}" /> | 
<a class="loginbutton" href="/$lang/pwdrestore">{$i18['LOST_PWD']}</a>
</form> 
FRM;
} 
?> 
</div>
<?php 
echo show_big_button('demo', $btn1, $btn1_link, $btn1_link, false);
echo show_big_button('buy', $btn2, '/^(buy|p_\d\d)$/is', "/$lang/$buy_page", true);
?>
<div id="leftColumn">
<?php	echo $session_prompt ?>

<?php
if(preg_match("/^man\d$/is", $main)){
    echo "<h5>".$i18['MAN_TOPICS']."</h5>";
    for($i=0; $i<=4; $i++){
        echo "<p>".anchor("man$i")."<img class=\"point\" src=\"/i/ico.gif\" alt=\"\"/> <br/><b>".$i18["MAN_$i"]."</b></a></p>\n";
    }
    $pdf_path="mobi/html/amax-manual-$lang.pdf";
    echo "<p><a href=\"$pdf_path\"><img class=\"point\" src=\"/i/ico.gif\" alt=\"\"/> <br/><b>".$i18['MAN_PDF']." (PDF ".
        fsize_human($pdf_path).")</b></a></p>\n";
}
else{
    if($show_topics){
		echo <<<PROD_INFO
<div id="productInfo">
<p><b>Astromaximum {$GLOBALS['amax']['year']}</b></p>
<p><b>+ {$GLOBALS['amax']['city_count']} {$i18['PRODINFO_CITY']}</b></p>
<p>{$i18['PRODINFO_VERSION']} {$GLOBALS['amax']['version']}<br/>
{$i18['PRODINFO_DATE']} {$GLOBALS['amax']['release_date']}</p>
<p><b>{$i18['PRODINFO_PRICE']} \${$GLOBALS['amax']['price']}</b></p>
</div>
<!--img class="main_scr" src="/i/p990i_2_bis2.jpg"/-->
<img class="main_scr" src="/i/diamond_1.jpg"/>
PROD_INFO;
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
				if($matches[2]==0 || $user_ok){ # grant access only to 0_*.php if not guest
	            prepare_topic($matches, $main);
	         }
	         else{
					reg_warning($i18['PAGE_READTHEMES']);
				}
			}
			else{
				include($fn);
			}
		}
		else{
			echo "<h3>{$i18['PAGE_NOT_FOUND']}</h3>\n";
		}
	} 
?>

</div><!-- end content div -->
<div id="bottom">
<div id="banners">
<!--LiveInternet counter--><script type="text/javascript"><!--
document.write("<a href='http://www.liveinternet.ru/click' "+
"target=_blank><img src='http://counter.yadro.ru/hit?t26.11;r"+
escape(document.referrer)+((typeof(screen)=="undefined")?"":
";s"+screen.width+"*"+screen.height+"*"+(screen.colorDepth?
screen.colorDepth:screen.pixelDepth))+";u"+escape(document.URL)+
";h"+escape(document.title.substring(0,80))+";"+Math.random()+
"' alt='' title='LiveInternet counter' "+
"border='0' width='88' height='15'><\/a>")
//--></script><!--/LiveInternet-->
</div>
<div id="contact">
<p><b>S&amp;W Axis</b></p>
<p>Kiev, Ukraine</p>
<img src="/i/email.png"/>
<p>Phone: +38(096)1188888</p>
<p>Skype ID: astromaximum</p>
</div>
<p>Copyright &copy;
2007-2009
S&amp;W Axis. All rights reserved.   &nbsp;&nbsp;    <a href="http://goglus.com">design goglus</a></p></div>
</body>
</html>

<?php
ob_end_flush();

function show_big_button($id, $label, $check_page, $link_page, $is_regexp){
	global $main, $lang_;
	$style='';
	$enable = 0;
	if ($is_regexp) {
		if (!preg_match($check_page, $main))
			$enable = 1;
	}
	else {
		if(strcmp($main, $check_page))
			$enable = 1;
	}
	if ($enable)
		$label="<a href=\"$link_page\">".$label.'</a>';
	else
		$style=' style="color:rgb(133,195,224)"';
	return "<div id=\"$id\"$style>$label</div>\n";
}

function prepare_topic($matches, $main){
    global $lang_, $i18;
    $nums=explode('_', $main);
    $num = $nums[0]? $nums[0]: $nums[1];
    include($matches[0]);
    $topic1=$topic; $body1=$body;
    if($matches[2]+$matches[3]){
        if($matches[2]){
            $fn0=$matches[1].$matches[2].'_0.php';
        }
        else{
            $fn0=$matches[1].$matches[3].'_0.php';
            
        }
        if(file_exists($fn0)){
            include($fn0);
            $topic = $i18["THEME_$num"]." - $topic";
            echo "<h4>$topic</h4>\n$body";
            if(strcmp($main, '0_0')){
                echo "<p><a href=\"0_".$main{0}."\"><strong>{$i18['BACK_TOPIC']}</strong></a><br/></p>\n";
            }
        }
    }
     echo "<h4>$topic1</h4>\n$body1";
}

function print_menu($page, $text, $is_delim)
{
	global $lang, $i18;
	echo "<a href=\"/$lang/$page\">{$i18[$text]}</a>";
	if ($is_delim) echo ' | ';
}

?>
