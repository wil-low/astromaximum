<?php
if(!isset($EXEC)) die("Access restricted");
$device = '&lt;Unknown&gt;';
if (isset($_GET['d']) && preg_match('/^\w+$/', $_GET['d']))
	$device = $_GET['d'];
echo 'Sorry, Astromaximum site does not currently support ' . $device . ' device.';
unset($_SESSION['nav']);
