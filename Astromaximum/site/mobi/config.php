<?php
# A better method than turning off error_reporting() is to turn off 'display_errors', 
# and use 'log_errors' and 'error_log'. This way developers can still get errors from misbehaving applications, and it won't trouble the user. <http://us2.php.net/manual/en/ref.errorfunc.php>
#error_reporting(E_NONE);
error_reporting(E_ALL);

$sett=array(
	'is_online'=>(isset($_SERVER['REMOTE_ADDR']) && strcmp($_SERVER['REMOTE_ADDR'],"127.0.0.1")),

	'use_smtp'=>true,
	'smtp_host'=>"smtp.astromaximum.mobi",
	'smtp_user'=>"web42p2",
	'smtp_pass'=>"TWmvvIWx",
	'mail_office'=>'office@astromaximum.mobi',
	'mail_site'=>'http://astromaximum.mobi',
	'noreply'=>'noreply@astromaximum.de',
	
	'def_cities'=>array('m.Olympos'),
	'demo_cities'=>array("London", "New York", "Moscow", "Kiev"),
	
	'demo_login'=>'123456789',
	'demo_pass'=>'012345678',
	
	'city_count'=>711,
	'price'=>'$60',
	'restore'=>"mobi/dl/source/restore", # pass restore mails folder
	'min_demo_year'=>2005,

	'DB_SERVER'=>'localhost',
	'DB_NAME'=>'usr_web42_1',
	'DB_PORT'=>'3306',
	
	'DB_SUPERUSER'=>'web42',
	'DB_SUPERUSER_PWD'=>'r6OvIJkV2U',
	'DB_USER'=>'user',
	'DB_USER_PWD'=>'user',
    
);

sort($sett['demo_cities']);
$GLOBALS['amax']=$sett;
?>