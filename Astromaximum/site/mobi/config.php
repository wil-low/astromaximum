<?php
# A better method than turning off error_reporting() is to turn off 'display_errors', 
# and use 'log_errors' and 'error_log'. This way developers can still get errors from misbehaving applications, and it won't trouble the user. <http://us2.php.net/manual/en/ref.errorfunc.php>
#error_reporting(E_NONE);
error_reporting(E_ALL);

$sett=array(
	'is_online'=>(isset($_SERVER['REMOTE_ADDR']) && strcmp($_SERVER['REMOTE_ADDR'],"127.0.0.1")),

	'use_smtp'=>true,
	'smtp_host'=>"smtp-2.1gb.ua",
	'smtp_user'=>"u7871",
	'smtp_pass'=>"d7ee8da9",
	'mail_office'=>'office@astromaximum.com',
	'mail_site'=>'http://astromaximum.com',
	'mail_site_mobi'=>'http://mobi.astromaximum.com',
	'noreply'=>'noreply@astromaximum.com',
	
	'def_cities'=>array('m.Olympos'),

	'demo_email'=>'demo@astromaximum.com',
	'demo_login'=>'123456789',
	'demo_pass'=>'012345678',
	
	'city_count'=>711,
	'price'=>'$60',
	'restore'=>"mobi/dl/source/restore", # pass restore mails folder
	'min_demo_year'=>2005,

	'DB_SERVER'=>'mysql300.1gb.ua',
	'DB_NAME'=>'gbua_x_astroc8a',
	'DB_PORT'=>'3306',
	
	'DB_SUPERUSER'=>'gbua_x_astroc8a',
	'DB_SUPERUSER_PWD'=>'f3cf9cc0',
	'DB_USER'=>'user',
	'DB_USER_PWD'=>'user',
    
);

$GLOBALS['amax']=$sett;
?>
