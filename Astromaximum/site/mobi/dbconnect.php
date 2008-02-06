<?php
/* FTP account 
astromaximumcom a2a0SL2H
*/
if(strcmp($_SERVER['SERVER_NAME'],"localhost")==0){ // local=true
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
	$pwd=md5($pwd);
	$stat=sprintf("SELECT id,realname FROM customers WHERE name=%s AND hash=%s",
		quote_smart($user),quote_smart($pwd));
	$sth=mysql_query($stat);
	if(mysql_num_rows($sth)==1){
		$row=mysql_fetch_row($sth);
		$_SESSION['uid']=$row[0];
		$_SESSION['username']=$row[1];
		$_SESSION['pwd']=$pwd;
		$res=true;
	}
/*	else{
		sleep(2);
	}
*/
	mysql_free_result($sth);
	return $res;
}

function logout(){
	$_SESSION = array();
	// If it's desired to kill the session, also delete the session cookie.
	// Note: This will destroy the session, and not just the session data!
	if (isset($_COOKIE[session_name()])) {
	    setcookie(session_name(), '', time()-42000, '/');
	}
	// Finally, destroy the session.
	session_destroy();
}

function check_access(){
	$stat=sprintf("SELECT role FROM customers WHERE id=%s AND hash=%s",
		quote_smart($_SESSION['uid']),quote_smart($_SESSION['pwd']));
//	echo $stat;
	$sth=mysql_query($stat);
	if(mysql_num_rows($sth)==1){
		$row=mysql_fetch_row($sth);
		return $row[0]; 
	}
	return -1;
}