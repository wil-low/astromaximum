<?php
/*  
	function authenticate() {
    header('WWW-Authenticate: Basic realm="Test Authentication System"');
    header('HTTP/1.0 401 Unauthorized');
    echo "You must enter correct username & password\n";
    exit;
  }
 
  if (!isset($_SERVER['PHP_AUTH_USER']) ||
      ($_POST['SeenBefore'] == 1 && $_POST['OldAuth'] == $_SERVER['PHP_AUTH_USER'])) {
   authenticate();
  } 
  else {
   echo "<p>Welcome: {$_SERVER['PHP_AUTH_USER']}<br />";
   echo "Previous login: {$_REQUEST['OldAuth']}";
   echo "<form action='{$_SERVER['PHP_SELF']}' METHOD='post'>\n";
   echo "<input type='hidden' name='SeenBefore' value='1' />\n";
   echo "<input type='hidden' name='OldAuth' value='{$_SERVER['PHP_AUTH_USER']}' />\n";
   echo "<input type='submit' value='Re-authorize' />\n";
   echo "</form></p>\n";
  }
*/
include('lang.php');
?>
<!--<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">-->
<html>
<head>
<title>About us - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="">
<meta name="date" content="2007-09-16T18:26:35+0300">
<meta name="copyright" content="">
<meta name="keywords" content="">
<meta name="description" content="">
<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8">
<meta http-equiv="content-style-type" content="text/css">
</head>
<body>

<?php
include('nav.php');
emit_nav1();
?>

<center><h3>What is Astromaximum?</h3></center>

<?php
emit_nav2();
?>
