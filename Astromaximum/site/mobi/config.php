<?php
$sett=array(
	'is_online'=>(isset($_SERVER['REMOTE_ADDR']) && strcmp($_SERVER['REMOTE_ADDR'],"127.0.0.1")),

	'use_smtp'=>true,
	'smtp_host'=>"smtp.astromaximum.mobi",
	'smtp_user'=>"web42p2",
	'smtp_pass'=>"TWmvvIWx",
	'mail_office'=>'office@astromaximum.mobi',
	'mail_site'=>'http://astromaximum.mobi',
	
	'def_cities'=>array('m.Olympos'),
	'demo_cities'=>array("London", "New York", "Moscow", "Kiev"),
	
	'demo_login'=>'123456789',
	'demo_pass'=>'012345678',
	
	'city_count'=>711,
	'price'=>'$60',
	'restore'=>"mobi/dl/source/restore", # pass restore mails folder
);

sort($sett['demo_cities']);
$GLOBALS['amax']=$sett;
?>