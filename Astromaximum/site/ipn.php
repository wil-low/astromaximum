<?php
$EXEC = 1;
include_once('mobi/config.php');
include_once('mobi/amtools.php');

$body = '';
foreach ($_POST as $key => $value) {
	$body .= $key . ' => ' . $value . "\n";
}

if (preg_match('/^\/ipn\/(Plimus|ShareIt)$/', $_SERVER['REQUEST_URI'], $matches)) {
	$body .= "IPN $matches[1] received:\n\n" . $body;
	event_send ("IPN $matches[1]", $body);
}
else {
	echo $body;
	event_send ("IPN invalid", "Unknown IPN received\n" . $body);
}
?>
