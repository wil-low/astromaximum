<?php
$TIME_MIN=3; // min time to reload page again
$ACCESS_ON=5; // access is checked when >
$TIME_DEL=60*60; // ip records will be deleted if older than this period
function allow_ip($pageid){
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
	if(ip_ok($ip, $pageid)){
		return;
	}
	$sess=session_name().'='.session_id();
	header("HTTP/1.0 401 Restricted");
	echo <<<EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Astromaximum</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<script type="text/javascript">
<!--
	function findObj(id) {
	  return (document.all?document.all[id]:document.getElementById(id));
	}

	function checkdata(){
		frm=findObj('fip');
		frm.submit();
	}
-->
</script>
</head>
<body>
<div style="text-align:center; vertical-align:middle;">
<form name="fip" action="{$_SERVER['REQUEST_URI']}" method="post">
<p>Введите символы, указанные на рисунке:</p>
<p><img src="mobi/kcaptcha/?$sess"><br/>
<input name="p_ipcaptcha" type="text" maxlength="6" size="6"/>
<input name="action" type="button" value="OK" onclick="checkdata()"/>
</p>
</form>
</div>
</body>
</html>
EOF;
	exit;
}

function ip_ok($ip, $pageid){
	global $TIME_MIN, $ACCESS_ON, $TIME_DEL;
	$result=true;
	$stat=sprintf("SELECT tm_first, tm_block, accessed, TIMESTAMPDIFF(SECOND,tm_first,tm_last)/accessed ".
		"FROM ipblock WHERE ip=%s AND pageid=%s",	$ip,$pageid);
	$sth=mysql_query($stat);
	if(!$sth) echo mysql_error();
	if($sth){
		if(mysql_num_rows($sth)==1){
			$row=mysql_fetch_row($sth);
#			print_r($row);
			if($row[2]>$ACCESS_ON){
				return $row[3]>$TIME_MIN;
			}
			$stat=sprintf("UPDATE ipblock SET accessed=accessed+1 WHERE ip=%s AND pageid=%s",
				$ip,$pageid);
			$sth=mysql_query($stat);
			if(!$sth) echo mysql_error();
		}
		else{ //insert new ip
//			$stat=sprintf("DELETE FROM ipblock WHERE tm_first<TIMESTAMPADD(SECOND, $TIME_DEL, NOW())");
//			$sth=mysql_query($stat);
			$stat=sprintf("INSERT INTO ipblock(ip, pageid, tm_first, accessed) ".
				"VALUES (%s, %s, CURRENT_TIMESTAMP, 1)", $ip, $pageid);
			$sth=mysql_query($stat);
			if(!$sth) echo mysql_error();
		}
	}
	else{
		echo mysql_error();
	}
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
