<?php
$DB_SERVER='localhost';
$DB_NAME='amax';
$DB_PORT='3306';

$DB_SUPERUSER='root';
$DB_SUPERUSER_PWD='toor';
$DB_USER='user';
$DB_USER_PWD='user';

$DIR_SOURCE='source';
$DIR_INBOX='inbox';
$DIR_FILES='files';

mysql_pconnect( $DB_SERVER, $DB_SUPERUSER, $DB_SUPERUSER_PWD );
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
	$stat=sprintf("SELECT id,name FROM customers WHERE name=%s AND hash=%s",
		quote_smart($user),quote_smart($pwd));
	$sth=mysql_query($stat);
	if(mysql_num_rows($sth)==1){
		$row=mysql_fetch_row($sth);
		$_SESSION['uid']=$row[0];
		$_SESSION['username']=$row[1];
		$res=true;
	}
	mysql_free_result($sth);
	return $res;
}

?>
