<?php
function allow_ip(){
	if(ip_ok()){
		return;
	}
	else{
		header("HTTP/1.0 403 Prohibited");
		echo "IP blocked: ".$_SERVER['REMOTE_ADDR'];
		exit;
	}
}

function ip_ok(){
	$ip=$_SERVER['REMOTE_ADDR'];
	$stat=sprintf("SELECT id,realname FROM customers WHERE name=%s AND hash=%s AND active>0",
		quote_smart($user),quote_smart($pwd2));
	$sth=mysql_query($stat);
	if(mysql_num_rows($sth)==1){
		$row=mysql_fetch_row($sth);
		$_SESSION['uid']=$row[0];
		$_SESSION['username']=$row[1];
		$_SESSION['pwd']=$pwd1;
		$res=true;
	}
	mysql_free_result($sth);
	
}

