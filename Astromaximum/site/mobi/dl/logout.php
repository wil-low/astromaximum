<?php
include_once('../dbconnect.php');
header("Location: ".$_SERVER['HTTP_REFERER']);
logout();
?>