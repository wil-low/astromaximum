<?php
$EXEC = 1;
include_once('mobi/config.php');
include_once('mobi/amtools.php');
include_once('mobi/dbconnect.php');

$ipn_log_file = '.ipn_results.log';

if (preg_match('/^\/ipn\/(Plimus|ShareIt)$/', $_SERVER['REQUEST_URI'], $matches)) {
	$system = $matches[1];
}
else {
	ipn_log ("IPN invalid", "Unknown IPN received on {$_SERVER['REQUEST_URI']}\n" . 
		implode ("\n", $_POST));
	return;
}

$data = implode ("\n", $_POST);

/* 
//test
$data = file_get_contents ("mobi/paypal/shareit.xml");
echo($data);
$system = 'ShareIt';
*/

$body = "IPN $system received:\n\n" . $data;
switch ($system) {
	case 'Plimus':
		$order_info['paymode'] = 6; # Plimus
		$plimus_tags = array (
			'referenceNumber' => 'txn_id',
		#	'PurchaseItem/NotificationNo' => 'notif_id', # or RunningNo ?
			'transactionType' => 'status',
			'transactionDate' => 'order_date',
			'invoiceAmount' => 'order_total',
			'accountId' => 'payer_id',
			'email' => 'payer_email',
			'firstName' => 'first_name',
			'lastName' => 'last_name',
			'address1' => 'street',
			'city' => 'city',
			'state' => 'state',
			'zipCode' => 'zip',
			'country' => 'country',
			'contractName' => 'item_name',
		#	'' => 'paymode',
			'paymentMethod' => 'payment_descr',
			'currency' => 'currency',
		);
		$msg = from_array ($_POST, $plimus_tags);
		break;
	case 'ShareIt':

		$order_info['paymode'] = 7; # share-it!

		$shareit_tags = array(
			'Purchase/PurchaseId' => 'txn_id',
			'Purchase/PurchaseItem/NotificationNo' => 'notif_id', # or RunningNo ?
			'Purchase/PaymentStatus' => 'status',
			'Purchase/PurchaseDate' => 'order_date',
			'Purchase/PurchaseItem/ProductSinglePrice' => 'order_total',
		#	'' => 'payer_id',
			'Purchase/CustomerData/BillingContact/Email' => 'payer_email',
			'Purchase/CustomerData/BillingContact/FirstName' => 'first_name',
			'Purchase/CustomerData/BillingContact/LastName' => 'last_name',
			'Purchase/CustomerData/BillingContact/Address/Street1' => 'street',
			'Purchase/CustomerData/BillingContact/Address/City' => 'city',
			'Purchase/CustomerData/BillingContact/Address/State' => 'state',
			'Purchase/CustomerData/BillingContact/Address/PostalCode' => 'zip',
			'Purchase/CustomerData/BillingContact/Address/Country' => 'country',
			'Purchase/PurchaseItem/ProductName' => 'item_name',
		#	'' => 'paymode',
			'Purchase/CustomerData/CustomerPaymentData/PaymentMethod' => 'payment_descr',
			'Purchase/CustomerData/CustomerPaymentData/Currency' => 'currency',
		);
		$xml = simplexml_load_string($data);
		$xml = $xml->OrderNotification;
		$msg = from_xml ($xml, $shareit_tags);
		$order_info['payer_id'] = 'N/A';
		break;
}

$body .= ipn_dump($order_info);
if ($msg)
	$body .= "$system parser error: " . $msg . "\n\n";
if (!insert_order ($order_info)) {
	$body .= "SQL error: " . mysql_error() . "\n"; 
}
ipn_log ("IPN $system", $body);
	
function from_xml ($xml, $xpath_array) {
	global $order_info;
	$message = '';
	foreach ($xpath_array as $key => $value) {
		if ($result = $xml->xpath ($key)) {
			$order_info[$value] = (string)$result[0];
		}
		else {
			$message .= 'Element not found: ' . $key . "\n";
		}
	}
	return $message;
}

function from_array ($array, $key_array) {
	global $order_info;
	$message = '';
	foreach ($key_array as $key => $value) {
		if (isset($array[$key])) {
			$order_info[$value] = $value;
		}
		else {
			$message .= 'Key not found: ' . $key . "\n";
		}
	}
	return $message;
}	

function ipn_log ($title, $text) {
	// Write to log
	global $ipn_log_file;
	$fp=fopen($ipn_log_file,'a');
	fwrite($fp, "\n---- " . $title . ' ' . strftime('%Y-%m-%d %H:%M:%S') . " ----\n" . $text . "\n\n"); 
	fclose($fp);  // close file
	event_send ($title, $text);
}

function ipn_dump ($arr) {
	$result = "\n";
	foreach ($arr as $key => $value) {
		$result .= "$key => '$value'\n";
	}
	return $result;
}
?>
