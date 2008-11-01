<?php
    $EXEC=1;
    include_once('mobi/dbconnect.php');
    $user=''; $pwd='';
    if(isset($_GET['email']))
        $user=$_GET['email'];
    if(isset($_GET['pwd']))
        $pwd=$_GET['pwd'];
	$pwd1=pwd_convert1($user, $pwd);
	$pwd2=pwd_convert2($pwd1);
	echo "Email: $user<br/>Pwd: $pwd2";
?>