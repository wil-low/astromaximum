<?php
if(!isset($EXEC)) die("Access restricted");
reject2index();
include_once('mobi/amtools.php');
lang_load("source");
$errors=0;
function yesno($val){
	global $errors;
	if($val){
		return '<span class="fine">YES</span>';
	}
	else{
		$errors++;
		return '<span class="alert">NO</span>';
	}
}
?>
<h4>Environment check</h4>
<table border="1" cellspacing="0">
<tr><th>Property</th><th>Status</th></tr>

<?php
	$perl=find_perl();
	$env=array();
	$env['error_reporting']=ini_get('error_reporting');
	$env['register_globals=off']=yesno(!ini_get('register_globals'));
	
	$p='mobi/dl/';
	$env['files writable']=yesno(is_writable($p.'files'));
	$env['inbox writable']=yesno(is_writable($p.'inbox'));
	$env['source writable']=yesno(is_writable($p.'source'));
	$env['restore writable']=yesno(is_writable($GLOBALS['amax']['restore']));
	$env['gen_amax.log writable']=yesno(is_writable($p.'gen_amax.log'));
	$ret=0;
	$outp=array();
	
	$apps=array(
		"sunrise"=>"mobi/sunrise",
		"fastjar"=>"mobi/fastjar -V",
		"bzip2"=>"bzip2 --help",
		"gzip"=>"gzip -V",
		"tar"=>"tar --version",
	);
	$value='';
	foreach($apps as $key=>$val){
		unset($outp);
		exec($val, $outp, $ret);
		$value.="$key ".yesno($ret==0).", ";
	}
	$env['tools']=$value;
	
	exec("$perl -c $p"."gen_amax.cgi", $outp, $ret);
	$env['gen_amax.cgi syntax']=yesno($ret==0);

	$key='source/*.comm';
	$env[$key]='';
	foreach (glob($p.$key) as $filename) {
	    $env[$key].=substr_replace(basename($filename), '', -5)." ";
	}
	if(strlen($env[$key])==0){
		$env[$key]='<span class="alert">NO</span>';
	}

	$env['jars']='';
	foreach (glob($p."source/*.jar") as $filename) {
	    $env['jars'].=substr_replace(basename($filename), '', -4)." ";
	}
	if(strlen($env['jars'])==0){
		$env['jars']='<span class="alert">no</span>';
	}

	$env['dl/html/.htaccess']=yesno(file_exists($p.'../html/.htaccess'));
	$key='Demo account';
	$stat="SELECT name,realname,hash,active FROM customers WHERE email='{$GLOBALS['amax']['demo_email']}'";
	$sth=mysql_query($stat);
	if($sth and ($row=mysql_fetch_row($sth))){
		$env[$key]="login: $row[0], realname: $row[1], password valid: ".
			yesno(strcmp($row[2], pwd_convert2(pwd_convert1($GLOBALS['amax']['demo_email'],
				$GLOBALS['amax']['demo_pass'])))==0).
			', active: '. yesno($row[3]); 
	}
	else{
		$env[$key]="<span class=\"alert\">MISSING</span>";
	}

	$key='Default cities';
	$adc=$GLOBALS['amax']['def_cities'];
	$dcit=explode(',', get_default_cities($adc));
//	print_r($dcit);
	$yprev=$GLOBALS['amax']['year']-1;
	$value='';
	for($i=0; $i<count($adc); $i++){
		$value.="$adc[$i] ";
		$sql="SELECT 1 FROM locations WHERE city_id = '$dcit[$i]' and year=$yprev";
		$sth=mysql_query($sql);
		$value.=yesno($sth && mysql_num_rows($sth));
		$value.=", ";
//		echo "$sql<br/>";
	}
	$env[$key]=$value;
/*
	$key='Demo cities';
	$adc=$GLOBALS['amax']['demo_cities'];
	$dcit=explode(',', get_default_cities($adc));
//	print_r($dcit);
	$yprev=$GLOBALS['amax']['year']-1;
	$value='';
	for($i=0; $i<count($adc); $i++){
		$value.="$adc[$i] ";
		$sql=isset($dcit[$i]) ?
		    "SELECT 1 FROM locations WHERE city_id = '$dcit[$i]' and year=$yprev": '';
		$sth=mysql_query($sql);
		$value.=yesno($sth && mysql_num_rows($sth));
		$value.=", ";
//		echo "$sql<br/>";
	}
	$env[$key]=$value;
*/
	foreach ($env as $key => $value) {
	 echo "<tr><td>$key</td><td>$value</td></tr>\n";
	}
?>
</table>
<?php if($errors) echo "<p class=\"alert\">There are $errors error(s)!!!</p>" ?>
