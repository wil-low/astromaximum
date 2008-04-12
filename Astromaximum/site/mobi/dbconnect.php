<?php
if(!isset($EXEC)) die("Access restricted");
/* FTP account 
astromaximumcom a2a0SL2H
*/
if(!isset($_SERVER) or strcmp($_SERVER['REMOTE_ADDR'],"127.0.0.1")==0){ // local=true
	$DB_SERVER='localhost';
	$DB_NAME='amax';
	$DB_PORT='3306';
	
	$DB_SUPERUSER='root';
	$DB_SUPERUSER_PWD='toor';
	$DB_USER='user';
	$DB_USER_PWD='user';
}
else{
	$DB_SERVER='localhost';
	$DB_NAME='usr_web42_1';
	$DB_PORT='3306';
	
	$DB_SUPERUSER='web42';
	$DB_SUPERUSER_PWD='vSZBWppx';
	$DB_USER='user';
	$DB_USER_PWD='user';
}

/*
	$DB_SERVER='mysql1.100ws.com';
	$DB_NAME='andivu_amax';
	$DB_PORT='3306';
	
	$DB_SUPERUSER='andivu_amax';
	$DB_SUPERUSER_PWD='toor';
	$DB_USER='user';
	$DB_USER_PWD='user';
*/

$DIR_SOURCE='dl/source';
$DIR_INBOX='dl/inbox';
$DIR_FILES='dl/files';

$conn=mysql_connect( $DB_SERVER, $DB_SUPERUSER, $DB_SUPERUSER_PWD );
mysql_select_db( $DB_NAME);

    // Функция экранирования переменных
function quote_smart($value)
{
  // если magic_quotes_gpc включена - используем stripslashes
  if (get_magic_quotes_gpc()) {
      $value = stripslashes($value);
  }
  // Если переменная - число, то экранировать её не нужно
  // если нет - то окружем её кавычками, и экранируем
  if (!is_numeric($value)) {
      $value = "'" . mysql_real_escape_string($value) . "'";
  }
  return $value;
}

function login($user,$pwd){
	$res=false;
	$pwd1=pwd_convert1($user, $pwd);
	$pwd2=pwd_convert2($pwd1);
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
	return $res;
}

function pwd_convert1($login, $pwd){
	$pwd=sha1($pwd.md5($login));
	$pwd=substr($pwd, 5, 16).substr($pwd, -16);
	return $pwd;
}

function pwd_convert2($pwd){
	$pwd=sha1($pwd);
	$pwd=substr($pwd, 11, 5).substr($pwd, 27).substr($pwd, 8, 3).substr($pwd, 16, 11);
	return $pwd;
}

function reject2index($url){
	$chac=check_access();
	if($chac!=0){
		redirect($url);
	}
}

function redirect($url){
	echo <<<EOF2
<html>
<head>
<meta http-equiv="refresh" content="0;url=$url">
</head>
</html>
EOF2;
	exit;
}

function check_access(){
	$pass=pwd_convert2($_SESSION['pwd']);
	$stat=sprintf("SELECT role FROM customers WHERE id=%s AND hash=%s",
		quote_smart($_SESSION['uid']),quote_smart($pass));
	$sth=mysql_query($stat);
	if(mysql_num_rows($sth)==1){
		$row=mysql_fetch_row($sth);
		return $row[0]; 
	}
	return -1;
}

function add_file($id, $type){
	$stat="INSERT INTO files(end_tm, id, type, user_id) VALUES (ADDTIME(NOW(), '2:00:00'), '".
		quote_smart($id)."', ".
		quote_smart($type).", ".
		quote_smart($_SESSION['uid']).")";
	return mysql_query($stat);	
}
