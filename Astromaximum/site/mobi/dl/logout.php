<?php
include_once('../dbconnect.php');
include_once('../lang.php');
$_SESSION = array();
if(isset($_COOKIE[session_name()])){
	setcookie(session_name(), '', time()-42000, '/');
}
session_destroy();
?>
<html>
<head>
<meta http-equiv="refresh" content="0;url=<?php echo $_SERVER['HTTP_REFERER'] ?>">
</head>
</html>