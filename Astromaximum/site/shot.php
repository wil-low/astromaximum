<?php
if (isset ($_GET['p'])) { // daily screen number
	$page = intval ($_GET['p']);
	$today = gmdate ('ymd');
	$ifile = "i/daily/$today-$page.png";
	header ("Content-type: image/png");
	if (file_exists ($ifile))
		readfile($ifile);
	exit;
}
// screenshot output
$EXEC=1;
include_once('mobi/lang.php');
lang_load("mobi/html");
$SHOT_MAX=16;
if(isset($_GET['n'])){
	$num=$_GET['n'];
	if($num>=1 && $num<=$SHOT_MAX){
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
<img src="/i/shot{$num}.png" alt="Image {$num}" width="240" height="300"/>
<table id="shotnav"><tr>
<td>
EOF;
    if($num>1){
        echo sprintf('<a href="/shot.php?%s&n=%02d">%d &lt;&lt;&lt;</a>', $lang_, $num-1, $num-1);
    }
    echo '</td><td>';
    if($num<$SHOT_MAX){
        echo sprintf('<a href="/shot.php?%s&n=%02d">&gt;&gt;&gt; %d</a>', $lang_, $num+1, $num+1);
    }
    echo <<<EOF1
</td>
</tr></table>
<textarea cols="50" rows="14" readonly="readonly">{$text}</textarea>
<p><a href="#" onclick="window.close()">[ {$i18['CLOSE']} ]</a></p>
</div>
</body></html>
EOF1;
	}
}
?>