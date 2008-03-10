<?php 
header("Content-type: image/png");
include_once("amtools.php");
include_once("lang.php");
$update_sec=1*3600;
//$lang='ru';
//$tmp=realpath(".")."/dl/source/info.dat";
//@unlink($tmp);
$imgfile=realpath(".")."/dl/source/info-$lang.png";
$now=gmdate('Y-m-d H:i:s');
//info_regenerate($imgfile, $now, $lang);
//echo $now;
//return;
//die($imgfile);
if(!file_exists($imgfile) or abs(filemtime($imgfile)-mktime())>$update_sec){
	info_regenerate($imgfile, $now, $lang);
//	$ttmp=fopen($tmp, "w");
//	fclose($ttmp);
}
$sun_degree=0;
readfile($imgfile);
return;

function info_regenerate($fname, $tm, $lang){
	global $sun_degree, $i18;
	$h=95; $w=150;
	$im = @imagecreatefrompng("dl/source/informer.png")
	      or die("Cannot Initialize new GD image stream");
	include_once("dbconnect.php");
	lang_load(realpath("html")); 
//	$transp = imagecolorallocatealpha($im, 255, 255, 255, 127);
//	$blue = imagecolorallocate($im, 197,225,239);
//	imagefilledrectangle($im, 0, 0, $w, $h, $blue);
//	$blue = imagecolorallocate($im, 197,225,239);
	//sun
//	$iplt = imagecreatefromgif("i/p0.gif");
//	imagecopy($im, $iplt, 4, 3, 0, 0, 12, 12);
	//moon
//	$iplt = imagecreatefromgif("i/p1.gif");
//	imagecopy($im, $iplt, 42, 3, 0, 0, 12, 12);
	//strings
//	$white = imagecolorallocate($im, 255, 255, 255);
	$black = imagecolorallocate($im, 0, 0, 0);
	$now=date("Y-m-d H:i");
	$px     = (imagesx($im) - imagefontwidth(2) * strlen($now)) / 2;
//	imagestring($im, 2, $px, $h-12, $now, $black);

	$font = 'DejaVuSans.ttf';
//	$title="В этот день:";
//	imagettftext($im, 7, 0, 80, 14, $black, $font, $title);
	if(record_in_range("_voc", $tm)){
		$text=$i18['VOC_MSG']." ";
	}
	else{
	//	$txt[1]=record_in_range("_vc", $tm);
		$sun_degree=record_in_range("_sundgr", $tm);
		$text=file(realpath(".").sprintf("/dl/source/interpret/$lang/Grade%01d.txt", $sun_degree/90));
		$text=array_filter($text, "find_dgr");
	//	print_r($sun);
		$text=trim(array_shift($text));
		$text=preg_replace("/.+?%\d+%\s+/is", "", $text);
	//	echo($sun);
	//	return;
	}
	mb_internal_encoding("UTF-8");
	$pos=0; $linew=27; $lineh=13; $linelim=5; $y=30;
	$len=mb_strlen($text);
	do{
		$tt=mb_substr($text, $pos, $linew);
		imagettftext($im, 7, 0, 4, $y, $black, $font, $tt);
		$y+=$lineh; $linelim--; $pos+=mb_strlen($tt);
	}while($pos<$len and $linelim>0);
	imagepng($im, $fname);
//	imagedestroy($iplt);
	imagedestroy($im);
//	echo "Regenerated $fname";
}

function find_dgr($var){
	global $sun_degree;
//	echo "$var\n";
	return strpos($var, "%$sun_degree%");
}
?> 
