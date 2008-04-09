<?php
if(!isset($EXEC)) die("Access restricted");
$to=1;
if(isset($_GET['to'])){
	$to=$_GET['to'];
	if(!is_numeric($to) || $to<0){
		$to=1;
	}
}

echo <<<EOF
Доступ заблокирован. Зайдите через $to мин.
EOF;

?>