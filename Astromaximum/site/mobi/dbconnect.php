<?php
if(!isset($EXEC)) die("Access restricted");

if(!$GLOBALS['amax']['is_online']){ // local=true
	$DB_SERVER='localhost';
	$DB_NAME='amax';
	$DB_PORT='3306';
	
	$DB_SUPERUSER='root';
	$DB_SUPERUSER_PWD='toor';
	$DB_USER='user';
	$DB_USER_PWD='user';
}
else{

	$DB_SERVER=$GLOBALS['amax']['DB_SERVER'];
    $DB_NAME=$GLOBALS['amax']['DB_NAME'];
	$DB_PORT=$GLOBALS['amax']['DB_PORT'];
	
	$DB_SUPERUSER=$GLOBALS['amax']['DB_SUPERUSER'];
	$DB_SUPERUSER_PWD=$GLOBALS['amax']['DB_SUPERUSER_PWD'];
	$DB_USER=$GLOBALS['amax']['DB_USER'];
	$DB_USER_PWD=$GLOBALS['amax']['DB_USER_PWD'];
/*
	$DB_SERVER='localhost';
	$DB_NAME='amax';
	$DB_PORT='3306';
	
	$DB_SUPERUSER='amaxroot';
	$DB_SUPERUSER_PWD='B4w0GxFUcT';
	$DB_USER='user';
	$DB_USER_PWD='user';
*/
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

function get_customer_data($str, $hash){ // out: id, name, realname, role
    global $EXEC;
	$arr=array();
    if(isset($EXEC) && (strlen($str)>0)){
        switch($EXEC){
            case 1: $fld='email'; break;
            case 2: $fld='name'; break;
            default: return $arr;
        }
        $stat=sprintf("SELECT id,name,email,role FROM customers WHERE $fld=%s AND active>0",
            quote_smart($str));
        if($hash)
            $stat.=sprintf(' AND hash=%s', quote_smart($hash));
//		echo $stat;
        $sth=mysql_query($stat);
        if(mysql_num_rows($sth)==1){
            $arr=mysql_fetch_array($sth);
        }
    }
	return $arr;
}

function login($user,$pwd){
	$res=false;
    $arr=get_customer_data($user, '');
    if(!count($arr))
        return false;
	$pwd1=pwd_convert1($arr[2], $pwd);
	$pwd2=pwd_convert2($pwd1);
    $arr=get_customer_data($user, $pwd2);
    
	if(count($arr)){
		$_SESSION['uid']=$arr[0];
		$_SESSION['username']=$arr[2];
		$_SESSION['pwd']=$pwd1;
		$res=true;
	}
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
	list($chac, $chac_pay)=check_access();
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

function check_access(){ // out: (role, paymode)
	$pass=pwd_convert2($_SESSION['pwd']);
	$stat=sprintf("SELECT role, paymode_id FROM customers WHERE id=%s AND hash=%s AND active>0",
		quote_smart($_SESSION['uid']),quote_smart($pass));
	$sth=mysql_query($stat);
	if(mysql_num_rows($sth)==1){
		$row=mysql_fetch_row($sth);
		return $row;
	}
	return array(-1,-1);
}

function add_file($id, $type){
	$stat="INSERT INTO files(end_tm, id, type, user_id) VALUES (ADDTIME(NOW(), '2:00:00'), '".
		quote_smart($id)."', ".
		quote_smart($type).", ".
		quote_smart($_SESSION['uid']).")";
	return mysql_query($stat);	
}
