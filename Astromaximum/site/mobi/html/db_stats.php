<?php
if(!isset($EXEC)) die("Access restricted");
reject2index("index.php?$lang_");
include_once('mobi/amtools.php');
lang_load("source");
$defyear=2007;
if(isset($_POST['year'])){
	$defyear=$_POST['year'];
}
$mode='env';
if(isset($_GET['mode'])){
	$mode=$_GET['mode'];
}
if(strcmp($mode,'env')==0){
	echo '<table>';
	echo '<tr><th>Property</th><th>Status</th></tr>';
	$env=check_env();
	foreach ($env as $key => $value) {
    echo "<tr><td>$key</td><td>$value</td></tr>\n";
	}
	echo '</table>';
}	
if(strcmp($mode,'data')==0){	
	echo '<p></p>';
	echo '<table>';
	echo '<tr><th>Country</th><th>Cities</th>';
	$sth=mysql_query("SELECT DISTINCT year FROM locations ORDER BY year");
	while($row=mysql_fetch_row($sth)){
		echo "<th>$row[0]</th>";
		$a_years[]=$row[0];
	}
	echo "</tr>";
	mysql_free_result($sth);
	$sth=mysql_query("SELECT countries.id, countries.name, COUNT(cities.id) FROM countries,cities".
		" WHERE cities.country_id=countries.id GROUP BY cities.country_id ORDER BY countries.name");
	$colored=1;
	while($row=mysql_fetch_row($sth)){
		echo "<tr";
		if($colored){
			echo ' bgcolor="#E5E5E5"';
		}
		$colored=1-$colored;
		echo "><td>{$row[1]}</td><td>{$row[2]}</td>";
		foreach($a_years as $i=>$value){
			$sth2=mysql_query(sprintf("select count(locations.id) from locations, cities where year=%d".
				" and locations.city_id=cities.id and cities.country_id=%d", $value, $row[0]));
			while($row2=mysql_fetch_row($sth2)){
				$bg='';
				if($row[2]>$row2[0]){
					$bg=" style=\"color:red\"";
				}
				echo "<td{$bg}>{$row2[0]}</td>\n";
			}
			mysql_free_result($sth2);
		}
		echo "</tr>\n";
	}
	mysql_free_result($sth);
	echo '</table>';
}

function check_env(){
	$perl=find_perl();
	$res=array();
	$p='mobi/dl/';
	$res['files writable']=yesno(is_writable($p.'files'));
	$res['inbox writable']=yesno(is_writable($p.'inbox'));
	$res['source writable']=yesno(is_writable($p.'source'));
	$res['gen_amax.log writable']=yesno(is_writable($p.'gen_amax.log'));
	$ret=0;
	$outp=array();
	exec("mobi/fastjar -V", $outp, $ret);
	$key="fastjar version";
	if($ret){
		$res[$key]='<font color="red">Error '.$ret.$php_errormsg.'</font>';
	}
	else{
		$res[$key]=$outp[0];
	}
	exec("$perl -c $p"."gen_amax.cgi", $outp, $ret);
	$res['gen_amax.cgi syntax']=yesno($ret==0);
	$key='source/*.comm';
	$res[$key]='';
	foreach (glob($p.$key) as $filename) {
	    $res[$key].=substr_replace(basename($filename), '', -5)." ";
	}
	if(strlen($res[$key])==0){
		$res[$key]='<font color="red">no</font>';
	}
	$res['jars']='';
	foreach (glob($p."source/*.jar") as $filename) {
	    $res['jars'].=substr_replace(basename($filename), '', -4)." ";
	}
	if(strlen($res['jars'])==0){
		$res['jars']='<font color="red">no</font>';
	}
	$res['dl/html/.htaccess']=yesno(file_exists($p.'../html/.htaccess'));
	global $DEMO;
	$key='Demo account';
	$stat="SELECT name, realname, hash FROM customers WHERE name='{$DEMO['login']}'";
	$sth=mysql_query($stat);
	if($sth and ($row=mysql_fetch_row($sth))){
		$res[$key]="login: $row[0], realname: $row[1], password valid: ".
			yesno(strcmp($row[2], pwd_convert2(pwd_convert1($DEMO['login'],$DEMO['pass'])))==0); 
	}
	else{
		$res[$key]="<font color=\"red\">MISSING</font>";
	}
	return $res;
}

function yesno($val){
	return $val? '<font color="green">YES</font>': '<font color="red">NO</font>';
}
?>