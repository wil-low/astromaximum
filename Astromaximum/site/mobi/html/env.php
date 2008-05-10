<?php
if(!isset($EXEC)) die("Access restricted");
reject2index("index.php?$lang_");
include_once('mobi/amtools.php');
lang_load("source");
function yesno($val){
	return $val? '<font color="green">YES</font>': '<font color="red">NO</font>';
}
?>
<table border="1" cellspacing="0">
<tr><th>Property</th><th>Status</th></tr>

<?php
	$perl=find_perl();
	$env=array();
	$p='mobi/dl/';
	$env['files writable']=yesno(is_writable($p.'files'));
	$env['inbox writable']=yesno(is_writable($p.'inbox'));
	$env['source writable']=yesno(is_writable($p.'source'));
	$env['gen_amax.log writable']=yesno(is_writable($p.'gen_amax.log'));
	$ret=0;
	$outp=array();
	
	$apps=array(
		"fastjar"=>"mobi/fastjar -V",
		"bzip2"=>"bzip2 --help",
		"gzip"=>"gzip -V",
		"gunzip"=>"gunzip -V",
		"tar"=>"tar --version",
	);
	foreach($apps as $key=>$value){
		unset($outp);
		exec($value, $outp, $ret);
		$key="$key found?";
		if($ret){
			$env[$key]='<font color="red">Error '.$ret.'</font>';
		}
		else{
			$env[$key]=yesno($ret==0);
		}
	}

	exec("$perl -c $p"."gen_amax.cgi", $outp, $ret);
	$env['gen_amax.cgi syntax']=yesno($ret==0);

	$key='source/*.comm';
	$env[$key]='';
	foreach (glob($p.$key) as $filename) {
	    $env[$key].=substr_replace(basename($filename), '', -5)." ";
	}
	if(strlen($env[$key])==0){
		$env[$key]='<font color="red">no</font>';
	}

	$env['jars']='';
	foreach (glob($p."source/*.jar") as $filename) {
	    $env['jars'].=substr_replace(basename($filename), '', -4)." ";
	}
	if(strlen($env['jars'])==0){
		$env['jars']='<font color="red">no</font>';
	}

	$env['dl/html/.htaccess']=yesno(file_exists($p.'../html/.htaccess'));
	$key='Demo account';
	$stat="SELECT name, realname, hash FROM customers WHERE name='{$GLOBALS['amax']['demo_login']}'";
	$sth=mysql_query($stat);
	if($sth and ($row=mysql_fetch_row($sth))){
		$env[$key]="login: $row[0], realname: $row[1], password valid: ".
			yesno(strcmp($row[2], pwd_convert2(pwd_convert1($GLOBALS['amax']['demo_login'],
			$GLOBALS['amax']['demo_pass'])))==0); 
	}
	else{
		$env[$key]="<font color=\"red\">MISSING</font>";
	}
	$key='Demo cities';
	$adc=$GLOBALS['amax']['demo_cities'];
	$dcit=get_default_cities($adc);
	$yprev=$GLOBALS['amax']['year']-1;
	$value='';
	for($i=0; $i<count($adc); $i++){
		$value.="$adc[$i] ";
		$value.=yesno(mysql_query("SELECT 1 FROM locations WHERE city_id = '$dcit[$i]' and year=$yprev"));
		$value.=", ";
	}
	$env[$key]=$value;

	foreach ($env as $key => $value) {
	 echo "<tr><td>$key</td><td>$value</td></tr>\n";
	}
?>
</table>
