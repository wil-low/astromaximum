<?php
if(!isset($EXEC)) die("Access restricted");
$device = '<Unknown>';
if (isset($_GET['d']))
	$device = $_GET['d'];
echo 'Sorry, Astromaximum sites do not currently support ' . $_GET['d'] . '.';
