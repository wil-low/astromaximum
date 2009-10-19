<?php 
if(!isset($EXEC)) die("Access restricted");

$cfields['paid']['captions'] 	= array ('BILL_NAME', 'BILL_DATE', 'BILL_NUM', 'BILL_EMAIL', 'BILL_USERNAME');
$cfields['paid']['inputs'] 		= array ('t', 't', 't', 't', 't');
$cfields['paid']['header']		= $i18['CONTACTS_00'];

function cform_paid($content) 
{
	$mail = mailtext_w_attach($GLOBALS['amax']['mail_bill'], '', 
		"Bank payment notification", $content);
	if($mail->ErrorInfo)
		return "Error sending mail: ".$mail->ErrorInfo;
	return '';
}
?>
