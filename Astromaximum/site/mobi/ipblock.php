<?php
$ACCESS_ON=5; // access is checked when >
$TIME_DEL=60*60; // ip records will be deleted if older than this period
$TIME_MIN=$TIME_DEL/$ACCESS_ON; // min time to reload page again
$MIN_BLOCKED=0;
function allow_ip($pageid, $is_mobi){
	global $MIN_BLOCKED;
	if(!$pageid){
		return;
	}
	$ip=quote_smart($_SERVER['REMOTE_ADDR']);
	$pageid=quote_smart($pageid);
	if(isset($_POST['p_ipcaptcha'])){
		$capok=is_capcha($_POST['p_ipcaptcha']);
		if($capok){
			$stat=sprintf("UPDATE ipblock SET accessed=1, tm_first=tm_last WHERE ip=%s AND pageid=%s",
				$ip,$pageid);
			$sth=mysql_query($stat);
			return;
		}
	}
	$MIN_BLOCKED=ip_ok($ip, $pageid);
	if($MIN_BLOCKED){
		global $i18;
		$msg=sprintf($i18['IP_BLOCKED'], $MIN_BLOCKED);
		if($is_mobi){
			$msg=<<<EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>No access - Astromaximum.mobi</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=30"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>
<div id="hdr" class="hdr">Astromaximum</div>
<div id="cont">$msg</div>
<div id="ftr"></div>
</body></html>
EOF;
		}
		return $msg;
	}
	return '';
}

function ip_ok($ip, $pageid){
	global $TIME_MIN, $ACCESS_ON, $TIME_DEL;
	$result=0;
	$stat=sprintf("UPDATE ipblock SET accessed=accessed+1, tm_last=NOW() WHERE ip=%s AND pageid=%s",
		$ip,$pageid);
	$sth=mysql_query($stat);
	$stat=sprintf("SELECT tm_first, TIMESTAMPDIFF(MINUTE, NOW(), tm_block), accessed, TIMESTAMPDIFF(SECOND,tm_first,tm_last)".
		"FROM ipblock WHERE ip=%s AND pageid=%s",	$ip, $pageid);
	$sth=mysql_query($stat);
	if($sth){
		if(mysql_num_rows($sth)==1){
			$row=mysql_fetch_row($sth);
			if($row[2]>=$ACCESS_ON){
				if($row[1]>0){
					return $row[1];
				}
				if($row[3]<$TIME_DEL){
					$stat=sprintf("UPDATE ipblock SET tm_block=TIMESTAMPADD(SECOND, $TIME_DEL, tm_first) WHERE ip=%s AND pageid=%s",
						$ip,$pageid);
					$sth=mysql_query($stat);
//				if(!$sth) echo mysql_error();
				}
			}
		}
		else{ //insert new ip
			$stat=sprintf("DELETE FROM ipblock WHERE TIMESTAMPADD(SECOND, $TIME_DEL, tm_first)< NOW()");
			$sth=mysql_query($stat);
			$stat=sprintf("INSERT INTO ipblock(ip, pageid, tm_first, accessed) ".
				"VALUES (%s, %s, CURRENT_TIMESTAMP, 1)", $ip, $pageid);
			$sth=mysql_query($stat);
//			if(!$sth) echo mysql_error();
		}
	}
//	else{
//		echo mysql_error();
//	}
	return $result;
}

function is_capcha($captcha){
	$res=false;
	if(count($_POST)>0){
		$res=isset($_SESSION['captcha_keystring']) && ($_SESSION['captcha_keystring'] ==  $captcha);
	}
	unset($_SESSION['captcha_keystring']);
	return $res;
}
?>
