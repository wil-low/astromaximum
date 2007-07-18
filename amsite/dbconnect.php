<?php
define('DB_SERVER','localhost');
define('DB_NAME','amax');
define('DB_PORT','3306');

define('DB_SUPERUSER','root');
define('DB_SUPERUSER_PWD','toor');
define('DB_USER','user');
define('DB_USER_PWD','user');

define('DIR_SOURCE','../source');
define('DIR_INBOX','../inbox');
define('DIR_FILES','../files');

mysql_pconnect( DB_SERVER, DB_SUPERUSER, DB_SUPERUSER_PWD );
mysql_select_db( DB_NAME);
?>
