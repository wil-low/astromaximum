<?php
if(!isset($EXEC)) die("Access restricted");
$DEMO=array('login'=>'123456789', 'pass'=>'012345678');

$DLIM=array(2, 8, 10); //download limited
  
function sess_start(){
	session_set_cookie_params(3600);
	session_start();
	session_register("username","uid", "pwd", "captcha_keystring");
}

function find_perl(){
	$perl="/opt/lampp/bin/perl";
	if(!file_exists($perl)){
		$perl="/usr/bin/perl";
	}
	return $perl;
}

function redirect($url){
	echo <<<EOF2
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<meta http-equiv="refresh" content="0;url=$url"/>
<title></title>
</head>
<body></body>
</html>
EOF2;
	exit;
}

function amtools_random($ye,$path, $ext){
	do{
		$id=''; $flag=1;
		for($i=0; $i<12; $i++){
			$id.=(int)(rand(0,9));
		}
		$id=$ye.$id;
		$fn="$path/$id$ext";
		if($ext){
			if(is_file($fn)){
				$flag=0;
			}
		}
		else{
			if(is_dir($fn)){
				$flag=0;
			}
		}
	}while(!$flag);
	return array($fn, $id);
}

$UNZIP="unzip %s -d %s -x *META-INF* "; # > /dev/null
$UNTAR="tar xvf %s -C %s";
$UNTBZ="tar xjvf %s -C %s";
$ZIP="fastjar %s ";

function jar($jarpath, $out, $manifest, $srcdir)
{
	return sprintf("%s/fastjar cvfm %s %s -C %s .", $jarpath, $out, $manifest, $srcdir);
}

function join_datafiles2($year, $destfile, $a_data) # year, destfile, data_listref
{
	$outf=fopen($destfile,'wb');
	$count=0;
	$data='';
	$a_len=array();
	foreach ($a_data as $i => $value) {
    	$data.=$value;
    	$a_len[$count]=strlen($value);
		$count++;
 	}
	fwrite($outf, pack('n',$year));
	fwrite($outf, pack('n',$count));
	foreach ($a_len as $i => $value) {
		fwrite($outf, pack('n',$value));
	}
	fwrite($outf, $data);
	fclose($outf);
}

function rm_all($dir)
{
	foreach(glob("$dir/*") as $fname){
		if (is_dir($fname))
			rm_all($fname);
		else
			@unlink($fname);
	}
	@rmdir($dir);
}

function select_cities($year, $ids, $destfile){
	$ids=trim($ids,',');
	$dir=dirname($destfile);
	if(!is_dir($dir)) mkdir($dir);
	$stat=sprintf("SELECT DISTINCT cities.name, data FROM cities, locations ".
		"WHERE cities.id IN (%s) AND city_id=cities.id AND year=%s".
		" ORDER BY cities.name",$ids,$year);
#	echo $stat;
	$sth = mysql_query($stat);
	$i=0;
	while($row = mysql_fetch_row($sth)){
		$data[$i++]=$row[1];		
		if(!isset($midlet_name)){
			$midlet_name=$row[0];
		}
	}
	mysql_free_result($sth);
#	echo $destfile;
	join_datafiles2($year, $destfile, $data);
	return $midlet_name;
}

function create_jar($year, $ids, $template_jar, $isdemo, $midlet_name, 
		$main_class, $common_file){
	global $DIR_FILES, $DIR_SOURCE, $UNZIP, $ZIP;
	$mypath=dirname(__FILE__);
	$dfil="$mypath/$DIR_FILES";
	$dsrc="$mypath/$DIR_SOURCE";
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="$dsrc/$fn";
//	mkdir($srcdir);
//	echo $srcdir;
	$server="http://".$_SERVER['SERVER_NAME'];
	$locfile='locations.dat';
	if($isdemo) $locfile='l.dat';
	$midlet_name_supposed=select_cities($year, $ids, "$dsrc/$fn/$locfile");
#	echo "<p>$dsrc/$fn/$locfile";
#	exit;
	$infile=fopen("$dsrc/template.jad","rb");
	$template = fread($infile, 1000000);
	fclose($infile);
	if(strcmp($midlet_name,'')){
		$fname="$midlet_name_'$ye";
	}
	else{
		$code="-".substr($fn,-4);
		$fname="Cities'$ye$code";
	}
	$template=str_replace('<NAME>', $fname, $template);
#	$jad=~s/<DESC>/$desc/isg;
	$template=str_replace('<JAR>', "$fname.jar", $template);
	$template=str_replace('<MAINCLASS>', $main_class, $template);
	$cmd=sprintf($UNZIP, $template_jar, "$dsrc/$fn");
	exec($cmd);
	$inf=fopen("/tmp/$fn.MF", 'wb');
	fwrite($inf, $template);
	fclose($inf);
	if(is_file($common_file)){
		$infile=fopen($common_file,"rb");
		$cfile = fread($infile, 1000000);
		fclose($infile);
		$comfile='common.dat';
		if($isdemo) $comfile='c.dat';
		$inf=fopen("$dsrc/$fn/$comfile", 'wb');
		fwrite($inf, $cfile);
		fclose($inf);
	}
	$inf=fopen("$dsrc/icons/".substr($year,-1).".png", 'rb');
	$icon=fread($inf,5000);
	fclose($inf);
	$inf=fopen("$dsrc/$fn/icon.png", 'wb');
	fwrite($inf,$icon);
	fclose($inf);
	$cmd=jar($mypath, "$dfil/$fn.r", "/tmp/$fn.MF", "$dsrc/$fn");
//	echo $cmd;
	exec($cmd);
//	usleep(500000);
//	emit_nav2();
//	exit();
	$inf=fopen("$dfil/$fn.d", 'wb');
	$asize= filesize("$dfil/$fn.r");
	$template.="MIDlet-Jar-Size: $asize\n";
	fwrite($inf, $template);
	fclose($inf);
	$server=$_SERVER['SERVER_NAME'];
	$template=preg_replace('/(MIDlet-Jar-URL: ).+?\n/is',"$1http://$server/mobi/data.php?r=$fn\n", $template);
	$inf=fopen("$dfil/$fn.t", 'wb');
	fwrite($inf, $template);
	fclose($inf);
	exec("rm -R $dsrc/$fn");
	unlink("/tmp/$fn.MF");
	return $fn;
}

/*
function is_mobile(){
    return 1;
// Lightweight device detection http://dev.mobi/node/472
    $mobile_browser = '0';

    if(preg_match('/(up.browser|up.link|mmp|symbian|smartphone|midp|wap|phone)/i',
        strtolower($_SERVER['HTTP_USER_AGENT']))){
        $mobile_browser++;
        }

    if((strpos(strtolower($_SERVER['HTTP_ACCEPT']),'application/vnd.wap.xhtml+xml')>0) or 
        ((isset($_SERVER['HTTP_X_WAP_PROFILE']) or isset($_SERVER['HTTP_PROFILE'])))){
        $mobile_browser++;
        }

    $mobile_ua = strtolower(substr($_SERVER['HTTP_USER_AGENT'],0,4));
    $mobile_agents = array(
        'w3c ','acs-','alav','alca','amoi','audi','avan','benq','bird','blac',
        'blaz','brew','cell','cldc','cmd-','dang','doco','eric','hipt','inno',
        'ipaq','java','jigs','kddi','keji','leno','lg-c','lg-d','lg-g','lge-',
        'maui','maxo','midp','mits','mmef','mobi','mot-','moto','mwbp','nec-',
        'newt','noki','oper','palm','pana','pant','phil','play','port','prox',
        'qwap','sage','sams','sany','sch-','sec-','send','seri','sgh-','shar',
        'sie-','siem','smal','smar','sony','sph-','symb','t-mo','teli','tim-',
        'tosh','tsm-','upg1','upsi','vk-v','voda','wap-','wapa','wapi','wapp',
        'wapr','webc','winw','winw','xda','xda-');

    if(in_array($mobile_ua,$mobile_agents)){
        $mobile_browser++;
        }
    if (strpos(strtolower($_SERVER['ALL_HTTP']),'OperaMini')>0) {
        $mobile_browser++;
        }
    if (strpos(strtolower($_SERVER['HTTP_USER_AGENT']),'windows')>0) {
        $mobile_browser=0;
    }
    if (strpos(strtolower($_SERVER['HTTP_USER_AGENT']),'iemobile')>0) {
                    $mobile_browser++;
    }
    return $mobile_browser;
}
*/

function get_default_cities($arr){
	if(is_array($arr)){
		$arr=implode("','", $arr);
	}
	$sth=mysql_query("SELECT id FROM cities WHERE name in ('$arr')");
	$ids='';
	while($row=mysql_fetch_row($sth)){
		$ids.="$row[0],";
	}
	mysql_free_result($sth);
	return substr($ids, 0, -1);
}

function midlet_create($type, $year, $lang, $param, $path2gen, $flag){
// $flag: 2= html, 1=text, 0=number 
// out - string with links
	global $DIR_FILES, $DIR_SOURCE, $i18, $EXEC;

	$timeout_offset=-24;
	$timeout_mins=2880;

	$str='';
	$perl=find_perl();
	if($EXEC==1){
		$dsrc="mobi/$DIR_FILES";
	}
	else{
		$dsrc=$DIR_FILES;
	}
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="$dsrc/$fn";
	$is_cal=true;
#	echo "$dsrc/$destfile";
	if(strcmp($type, "geo")==0){
		$cmd="$perl $path2gen/gen_amax.cgi geo- $year $lang $param $dsrc/$fn.r nomessjar";
	}
	else{
		$is_cal=true;
		if(!preg_match("/^(demo|tb)$/is", $type)){
			return '';
		}
		$cmd="$perl $path2gen/gen_amax.cgi $type $year $lang \"$param\" $dsrc/$fn.r $timeout_offset $timeout_mins nomessjar";
	}
//	echo $cmd;
	$ret=0;
	exec($cmd, $outp, $ret);
	if($ret){				
		$str.=$cmd.'<br/>';
		$str.=implode('<br/>',$outp);
		error_log($str);
		$str='';
	}
	else{
		if(!add_file($fn, $type{0}." $year $lang")){
			$str.=mysql_error();
			error_log($str);
			return '';
		}
		$id=$fn;
		$data_php="http://".$_SERVER['SERVER_NAME'];
		$abs_path='./';
		if(strpos($data_php, "mobi") === false){
			$data_php.="/mobi";
			$abs_path.='/mobi';
		}
		$url=$data_php.'/data.php?r='.$id;
		$jarsize=fsize_human("$abs_path/dl/files/$id.r");
		$jadsize=fsize_human("$abs_path/dl/files/$id.d");
		switch($flag){
			case 2:
		 		$url2=str_replace("?r", "?d", $url);
				if($EXEC==1){
					$str.="<h4>{$i18['PC_DL']}:</h4>";
					$str.="<a href=\"$url\">JAR ($jarsize)</a><br/>\n";
				}
				else{
					$str.="<h4>{$i18['PHONE_DL']}:</h4>";
				}
				$str.="<a href=\"$url2\">JAD ($jadsize)</a><br>\n";
		#				$url=str_replace("?d", "?t", $url);
		#				echo "<h4>{$i18['PHONE_DL']}:</h4>";
		#				echo "<a href='$url'>JAD</a><br>";
				$str.="<br/>{$i18['BOTH_FILES']}";
				$str.="<br/><br/><span class=\"alert\">{$i18['VALID_LINKS']}</span>";
				break;
			case 1:
		 		$url2=str_replace("?r", "?t", $url);
				$str="$url\n$url2\n";
				break;
			case 0:
				$str = $id;
		}
	}
	return $str; // 1st is JAR, 2nd is JAD
}

function fsize_human($fname){
	$size=filesize($fname);
	if(!$size)
		return 0;
	if($size < 1024)
		return "$size b";
	$size/=1024.;
	if($size < 1024)
		return sprintf("%0.1f Kb", $size);
	$size/=1024.;
	if($size < 1024)
		return sprintf("%0.1f Mb", $size);
}

function record_in_range($table, $tm){
	$stat=sprintf("SELECT * FROM %s WHERE %s BETWEEN start AND end LIMIT 1", $table, quote_smart($tm));
	$sth=mysql_query($stat);
	if(!$sth){
		echo mysql_error();
		return 0;
	}
	$res=mysql_num_rows($sth);
	if($res and strcmp($table, "_sundgr")==0){
		$res=mysql_fetch_row($sth);
		$res=$res[2];
	}
//	echo "$stat\n";
	mysql_free_result($sth);	
	return $res;
}

function mailtext_w_attach($to, $realname, $subject, $message){ # returns sent Mail object
    include_once("mobi/phpmailer/class.phpmailer.php");
    include_once("mobi/phpmailer/class.smtp.php");
    $from=$GLOBALS['amax']['mail_office'];
    
    $mail=new PHPMailer();
    $mail->SetLanguage("en", "mobi/phpmailer/");
    if($GLOBALS['amax']['is_online']){
        $mail->IsSMTP();
        $mail->SMTPAuth   = true;                  // enable SMTP authentication
        //  $mail->SMTPSecure = "ssl";                 // sets the prefix to the servier
        $mail->Host       = $GLOBALS['amax']['smtp_host'];// sets GMAIL as the SMTP server
        $mail->Port       = 25;                   // set the SMTP port 
        
        $mail->Username   = $GLOBALS['amax']['smtp_user'];
        $mail->Password   = $GLOBALS['amax']['smtp_pass'];
    } 
    $mail->From       = $from;
    
    $mail->FromName   = 'Astromaximum office';
    $mail->Subject    = $subject;
    //	echo $GLOBALS['amax']['restore'];
    if($tmpfname = tempnam($GLOBALS['amax']['restore'], "")){
    //		echo $tmpfname;
        if(strpos($tmpfname, $GLOBALS['amax']['restore'])){
            $handle = fopen($tmpfname, "w");
            fwrite($handle, $message);
            fclose($handle);
            $mail->AddAttachment($tmpfname, "message.txt","8bit", "text/plain");
        }
    }
    
    $message=preg_replace("/\-{4,}.+/s", "", $message);
    $mail->Body = $message;
    $mail->AddAddress($to,$realname);
    $mail->Send();
    return $mail;
}

function event_send($topic, $message){
    return mailtext_w_attach($GLOBALS['amax']['mail_event'], '', 'Amax event - '.$topic, $message);
}

function pwd_send($to, $login, $realname, $dl_limits, $pwd){
    if(!$realname) $realname="customer";
	$message=file_get_contents("mobi/dl/source/pwdrestore.mail");
	$message=str_replace('[site]', $GLOBALS['amax']['mail_site'], $message);
	$message=str_replace('[mobi]', $GLOBALS['amax']['mail_site_mobi'], $message);
	$message=str_replace('[realname]', $realname, $message);
	$message=str_replace('[email]', $to, $message);
	$message=str_replace('[login]', $login, $message);
	$message=str_replace('[pwd]', $pwd, $message);
	$message=str_replace('[dl_count]', $dl_limits[0], $message);
	$message=str_replace('[city_count]', $dl_limits[1], $message);
	$message=str_replace('[past_count]', $dl_limits[2], $message);
	return mailtext_w_attach($to, $realname, 'Astromaximum - new password', $message);
}

function confirmation_send($to, $realname, $passkey){
	global $lang;
    $message=file_get_contents("mobi/html/$lang/confirm.mail");
	if (!$message) return -1;
	$message=str_replace('[realname]', $realname, $message);
	$message=str_replace('[site]', $GLOBALS['amax']['mail_site'], $message);
	$message=str_replace('[key]', $passkey, $message);
	return mailtext_w_attach($to, $realname, 'Astromaximum - confirm registration', $message);
}

function get_try_count($id){ // get dl limit for current user, if $id==0
	if(intval($id)==0){
		$id=$_SESSION['uid'];
	}
	$stat=sprintf("SELECT dlcount0, dlcount1, dlcount2 FROM customers WHERE id=%d", quote_smart($id));
	$sth=mysql_query($stat);
//	global $DLIM;
	if($sth && ($row=mysql_fetch_row($sth))){
		return $row;
	}
	return array(0,0,0);	
}

function dec_try_count($id, $key){ // decrease dl limit by $key for current user, if $id==0
	if(intval($id)==0){
		$id=$_SESSION['uid'];
	}
	if(is_numeric($key)){
		$key="dlcount$key";
	}
	$stat=sprintf("UPDATE customers SET $key=$key-1 WHERE id=%d AND $key>0", quote_smart($id));
	return mysql_query($stat);
}

function tries_remained($tries, $limit) {
	global $i18;
    if($tries<0)
        return '';
    else
        return sprintf($i18['TRIES_REMAINED'], $tries, $limit);
}

function is_captcha($captcha){
	$res=false;
	if(count($_POST)>0){
		$res=isset($_SESSION['captcha_keystring']) && ($_SESSION['captcha_keystring'] ==  $captcha);
	}
	unset($_SESSION['captcha_keystring']);
	return $res;
}

function check_email_address($email) { # http://www.addedbytes.com/php/email-address-validation/
	// First, we check that there's one @ symbol, and that the lengths are right
	if (!ereg("^[^@]{1,64}@[^@]{1,255}$", $email)) {
		// Email invalid because wrong number of characters in one section, or wrong number of @ symbols.
		return false;
	}
	// Split it into sections to make life easier
	$email_array = explode("@", $email);
	$local_array = explode(".", $email_array[0]);
	for ($i = 0; $i < sizeof($local_array); $i++) {
		if (!ereg("^(([A-Za-z0-9!#$%&'*+/=?^_`{|}~-][A-Za-z0-9!#$%&'*+/=?^_`{|}~\.-]{0,63})|(\"[^(\\|\")]{0,62}\"))$", $local_array[$i])) {
			return false;
		}
	} 
	if (!ereg("^\[?[0-9\.]+\]?$", $email_array[1])) { // Check if domain is IP. If not, it should be valid domain name
		$domain_array = explode(".", $email_array[1]);
		if (sizeof($domain_array) < 2) {
			return false; // Not enough parts to domain
		}
		for ($i = 0; $i < sizeof($domain_array); $i++) {
			if (!ereg("^(([A-Za-z0-9][A-Za-z0-9-]{0,61}[A-Za-z0-9])|([A-Za-z0-9]+))$", $domain_array[$i])) {
				return false;
			}
		}
	}
	return true;
}

function hide_email($email){
    return 'mailto:'.$email;
}

function random9(){ // generate 9-digit randoms
    return sprintf('%09d', mt_rand(0, 999999999));
}

function show_payment_instructions($payment_id){ // print payment page
    global $lang, $lang_, $i18, $META_HEAD_ADD;
    $payment_id=sprintf('%02d', $payment_id);
    $tabs='<h4>'.sprintf ($i18['REGFORM_PAYMODE'], $GLOBALS['amax']['year']).'</h4><div class="tabs"><ul class="tabNavigation">';
	
	$tabs .= '<li><a class="" href="#">'.$i18['PAYMENT_02'].'</a></li>';
	$tabs .= '<li><a class="" href="http://astromaximum.com/wiki/doku.php/bill" target="_blank">'.$i18['PAYMENT_04'].'</a></li>';
	
/*	
    foreach($GLOBALS['amax']['paymodes'] as $key){
        $key2=sprintf('%02d', $key);
        $tabs.='<li><a class="" href="#a'.$key2.'">'.$i18['PAYMENT_'.$key2]."</a></li>\n";
    }
    $tabs.='</ul>';
    $META_HEAD_ADD = <<< EOF
<script type="text/javascript" src="/jquery-1.2.6.min.js"></script>
<script type="text/javascript">
<!--
$(document).ready(init);
function init() {
    var tabContainers = $('div.tabs > div'); // получаем массив контейнеров
    tabContainers.hide().filter('#a{$payment_id}').show();
    // далее обрабатывается клик по вкладке
    $('div.tabs ul.tabNavigation a').click(function () {
        tabContainers.hide(); // прячем все табы
        tabContainers.filter(this.hash).show(); // показываем содержимое текущего
        $('div.tabs ul.tabNavigation a').removeClass('selected'); // у всех убираем класс 'selected'
        $(this).addClass('selected'); // текушей вкладке добавляем класс 'selected'
        return false;
    }).filter('#a{$payment_id}').click();
}
//-->
</script>
EOF;

    echo $tabs;
	echo '<div id="#a00"></div>';
    foreach($GLOBALS['amax']['paymodes'] as $key){
        $key2=sprintf('%02d', $key);
        echo '<div id="a'.$key2.'" style="display:none">';
        $fn="mobi/html/p_$key2.php";
        if(file_exists($fn)){
            include($fn);
        }
        echo "</div>\n";
    }
    echo '</div>';
*/
    $tabs.='</ul>';
	echo $tabs;
}

function fill_input_str($id, $text){
    global $i18;
    return "<a href=\"javascript:void(0)\" onclick=\"fill_input('$id','$text')\" ".
        "alt=\"{$i18['FILL_INPUT']}\">$text</a>";
}
?>
