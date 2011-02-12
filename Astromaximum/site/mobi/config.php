<?php
# A better method than turning off error_reporting() is to turn off 'display_errors', 
# and use 'log_errors' and 'error_log'. This way developers can still get errors from misbehaving applications, and it won't trouble the user. <http://us2.php.net/manual/en/ref.errorfunc.php>
#error_reporting(E_NONE);
error_reporting(E_ALL & ~E_DEPRECATED);
$is_online = isset($_SERVER['REMOTE_ADDR']) && strcmp($_SERVER['REMOTE_ADDR'],"127.0.0.1");
$sett=array(
	'is_online'=>$is_online,

	'use_smtp'=>true,
	'smtp_host'=>"smtp-2.1gb.ua",
	'smtp_user'=>"u7871",
	'smtp_pass'=>"d7ee8da9",
	'mail_office'=>'office@astromaximum.com',
	'mail_site'=>'http://astromaximum.com',
	'mail_site_mobi'=>'http://mobi.astromaximum.com',
	'noreply'=>'noreply@astromaximum.com',
	
	'mail_event'=>'astromaximum@gmail.com',
	'mail_bill'=>'bix@astromaximum.com',

	'def_cities'=>array(
		'ru'=>array('m.Olympos', 'Kiev', 'Moscow'),
		'en'=>array('m.Olympos', 'London', 'New York'),
		'de'=>array('m.Olympos', 'Berlin', 'Vienna'),
	),

	'city_count'=>725, //update PRODINFO_CITY in ru.msg
	'price'=>'58.99',
	'version'=>'1.1.8 r634',
	'release_date'=>'2011/02/07',
	
	'restore'=>"mobi/dl/source/restore", # pass restore mails folder
	'min_demo_year'=>2000,
    
	'buy_enabled'=>0,
    'paymodes'=>array(7/*, 6, 4*/),

	'paypal_url'=>'https://www.paypal.com',
	'paypal_email'=>'paypal@astromaximum.com',

//	'paypal_url'=>'https://www.sandbox.paypal.com',
//	'paypal_email'=>'aivush_1217502939_biz@gmail.com',
);

$GLOBALS['amax']=$sett;

if ($is_online) {
	$GLOBALS['amax']['DB_SERVER']='mysql300.1gb.ua';
	$GLOBALS['amax']['DB_NAME']='gbua_x_astroc8a';
	$GLOBALS['amax']['DB_PORT']='3306';
	
	$GLOBALS['amax']['DB_SUPERUSER']='gbua_x_astroc8a';
	$GLOBALS['amax']['DB_SUPERUSER_PWD']='f3cf9cc0';
	$GLOBALS['amax']['DB_USER']='user';
	$GLOBALS['amax']['DB_USER_PWD']='user';
}
else {
	$GLOBALS['amax']['DB_SERVER']='localhost';
	$GLOBALS['amax']['DB_NAME']='amax';
	$GLOBALS['amax']['DB_PORT']='3306';
	
	$GLOBALS['amax']['DB_SUPERUSER']='root';
	$GLOBALS['amax']['DB_SUPERUSER_PWD']='toor';
	$GLOBALS['amax']['DB_USER']='amax';
	$GLOBALS['amax']['DB_USER_PWD']='amax-user';
}
?>
