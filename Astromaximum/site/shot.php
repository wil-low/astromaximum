<?php
$EXEC=1;
include_once('mobi/lang.php');
lang_load("mobi/html");
if(isset($_GET['n'])){
	$num=$_GET['n'];
	if($num>=0 && $num<16){
		$num=sprintf("%02d", $num);
		$text=$i18["SCRTEXT_$num"];
        $text=str_replace('\n',"\n", $text);
		echo <<<EOF
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
<title>#{$num}</title>
<link href="astro.css" rel="stylesheet" type="text/css"/>
</head>
<body>
<div class="shot">
<img src="/i/shot{$num}.png" alt=""/><br/><br/>
<textarea cols="45" rows="12" readonly="readonly">{$text}</textarea>
<p><a href="#" onclick="window.close()">[ {$i18['CLOSE']} ]</a></p>
</div>
</body></html>
EOF;
	}
}
?>