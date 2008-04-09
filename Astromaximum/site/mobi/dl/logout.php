<?php
$EXEC=5;
include_once('../lang.php');
sess_start();
$_SESSION = array();
if(isset($_COOKIE[session_name()])){
	setcookie(session_name(), '', time()-42000, '/');
}
session_destroy();
$url="/";
if(isset($_SERVER['HTTP_REFERER'])){
	$url=$_SERVER['HTTP_REFERER'];
}
?>
<html>
<head>
<meta http-equiv="refresh" content="0;url=<?php echo $url ?>">
</head>
</html>