<?php
include_once('../lang.php');
?>
<html>
<head>
<title>Cities database - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="Unknown">
<meta name="date" content="2007-11-16T21:22:45+0200">
<meta name="copyright" content="">
<meta name="keywords" content="">
<meta name="description" content="">
<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8">
<meta http-equiv="content-style-type" content="text/css">
<meta http-equiv="expires" content="0">
<link href="../style.css" rel="stylesheet" type="text/css">
</head>
<body>
<?php
include_once('nav.php');
emit_nav1();
$chac=check_access();
if(!$chac){ 
	echo "<br><p align=center>".$i18['DB_ACCESS']."</p>";
	emit_nav2();
	exit();
}
if($chac==1){
	emit_admin();
} 
include_once('../amtools.php');
?>
	<form method="post" action="upload.php" enctype="multipart/form-data">
		Upload what: 
		<select name="restype">
			<option value='geodata' selected>Geo data</option>
			<option value='tjar'>Archive template</option>
			<option value='tjad'>Descriptor template</option>
			<option value='demo'>Demo</option>
		</select><p>
		<input type="file" name="uploaded_file" value="" size="50" maxlength="80" />
<!--  	<input type="hidden" name="MAX_FILE_SIZE" value="300000" />-->
		<br><input type="submit" name="Action" value="Upload" />
	</form>
<?php	
	if(!isset($_FILES['uploaded_file'])|| !$_FILES['uploaded_file']['name']){
		emit_nav2();
		exit();
	}
	else{
		$fname=$_FILES['uploaded_file']['name'];
		$restype=$_POST['restype'];

		if(!strlen($restype) || !strlen($fname)){
			echo "error $fname";
			exit();
		}
		if(strcmp($restype,'geodata')==0){
			up_geodata($fname, 'zip');
		}
		if(strcmp($restype,'tjar')==0){
			up_res($fname, 1,'zip');
		}
		if(strcmp($restype,'tjad')==0){
			up_res($fname, 2,'jad');
		}
		if(strcmp($restype,'demo')==0){
			up_res($fname, 3,'jar');
		}
	}
	
function up_res($fname, $resid, $ext){
	check_ext($fname, $ext);
	$fn = $_FILES['uploaded_file']['tmp_name'];
	$FF0=fopen($fn,"rb");
	$data=fread($FF0, filesize($fn));
	fclose($FF0);
	mysql_query(sprintf("UPDATE source SET data=%s WHERE id=%s", quote_smart($data), $resid))
		 or upload_error("Update error: " . mysql_error());
	echo "<p>Uploaded successfully.";
}
	
function check_ext($fname, $ext){
	$myext=substr($fname,-4);
	if(strcmp($myext,'.'.$ext)/* && strcmp($ext,'.zip')*/){
		upload_error("Invalid archive: '$fname'",'');
	} 
}

function up_geodata($fname, $ext){
	global $DIR_INBOX, $UNZIP;
	check_ext($fname, $ext);
	$fh = $_FILES['uploaded_file']['tmp_name'];
	list($dir,$fn)=amtools_random(0,$DIR_INBOX,'');
	mkdir($dir);
	$cmd=sprintf($UNZIP, $fh, $dir);
	$res=exec($cmd);
//		echo "<pre>$res</pre>";
/*		if(!$res){
		echo "System(unzip) failure<br>";
		echo 'safe_mode = ' . ini_get('safe_mode') . "<br>";
		echo 'safe_mode_exec_dir = ' . ini_get('safe_mode_exec_dir') . "<br>";
	}		
*/
	$fn=glob("$dir/*.txt");
	if(count($fn)!=1){
		upload_error("TXT must be exactly one file in archive $dir/*.txt, not ".count($fn), $dir);
	}
	echo("<b>Reading $fn[0]</b><br><table style='font-size:9pt' border=1><tr><th>City</th>".
		"<th>Country</th><th>State</th><th>Year</th><th>TXT</th><th>Cities DB</th></tr>");
	$sthcou = "SELECT countries.id FROM countries WHERE countries.name=%s";
	$sthcit = "SELECT cities.id FROM cities,countries WHERE cities.name=%s AND country_id=%s AND state_id=%s";
	$sthstate = "SELECT states.id FROM states,countries WHERE states.name=%s AND country_id=%s";
	$sthcouins = "INSERT INTO countries(name,continent) VALUES (%s,%s)";
	$sthstateins = "INSERT INTO states(name,country_id) VALUES (%s,%s)";
	$sthcitins = "INSERT INTO cities(name,country_id,state_id) VALUES (%s,%s,%s)";
	$sthloc = "SELECT id FROM locations WHERE year=%s AND city_id=%s";
	$sthlocupd = "UPDATE locations SET data=%s WHERE id=%s";
	$sthlocins = "INSERT INTO locations(year,city_id,data) VALUES(%s,%s,%s)";
	$findex=fopen($fn[0],"r");
	$fn=glob("$dir/Data*.dat");
//  	echo count($fn);
//		emit_nav2();
//		exit();
	list($cou_count,$cit_count,$locins_count,$locupd_count,$state_count,$i)=array(0,0,0,0,0,0);
	$matches=array();
	while (!feof($findex)) {
		$cc = fgets($findex, 4096);
		$cc=preg_replace('/^\"/is','',$cc);
		$cc=preg_replace('/\"\s*$/is','',$cc);
		$cc=preg_replace('/\#.+/is','',$cc);
  	list($name, $country, $yr, $txtchk, $status, $state)=array('','',0,'','','');
		$rec=explode('|', $cc);
		if(count($rec)!=5){
			continue;
		}
		$name=$rec[0];
		$continent=$rec[4];
		$continent=preg_replace('/[\n\r]/is','',$continent);
		$name=preg_replace("/.+?\!/is",'',$name,1);
		$country=$rec[3];
		$country=preg_replace('/.+?\$/is','',$country,1);
		$state='';
		if(preg_match('/ - (.+)/is',$country,$matches)){
			$state=$matches[1];
			$country=preg_replace("/ - (.+)/is",'',$country,1);
		}
		$curfn=$fn[$i++];
		$FF0=fopen($curfn,"rb");
  	$tr='';
  	$locdata='';
  	$yr=fread($FF0,2);
  	$yr=current(unpack("S",$yr));
  	if($yr){
			fseek($FF0,8,0);
			$len=0;
			$len=fread($FF0,2);
			$len=current(unpack("n",$len));
			$len=fread($FF0,$len);
			fseek($FF0,0,0);
			$locdata=fread($FF0, filesize($curfn));
			fclose($FF0);
			$tst=$name;
/*				if($state){
				$tst.=", $state";
			}
*/				if(strcmp($len,$tst)==0){
				$txtchk="<b>OK</b>";
			}
			else{
				$txtchk="<font color=red>doesn't match, found <b>$len</b></font>";
			}
		}
		else{
			$txtchk="<font color=red>missing</font>";
		}
		$couid=0;
		$sth=mysql_query(sprintf($sthcou,quote_smart($country)));
		if(!mysql_num_rows($sth)){
			$que=sprintf($sthcouins,quote_smart($country),quote_smart($continent));
//			echo $que;
			$sth=mysql_query($que);
			$couid=mysql_insert_id();
			$country="<font color=red>$country</font>";
			++$cou_count;
		}
		else{	
			$couid=mysql_result($sth,0);
		}
		$stateid=0;
		if($state){
			$sth=mysql_query(sprintf($sthstate,quote_smart($state),$couid));
			if(!mysql_num_rows($sth)){
				$sth=mysql_query(sprintf($sthstateins,quote_smart($state),$couid));
				$stateid=mysql_insert_id();
				$state="<font color=red>$state</font>";
				++$state_count;
			}
			else{	
				$stateid=mysql_result($sth,0);
			}
		}
		$citid=0;
//			echo(sprintf($sthcit,quote_smart($name),$couid,$stateid).'<br>');
		$qq=sprintf($sthcit,quote_smart($name),$couid,$stateid);
//		echo "~$qq~\n";
		$sth=mysql_query($qq);
//		echo mysql_error();
		if(!mysql_num_rows($sth)){
			$sth=mysql_query(sprintf($sthcitins,quote_smart($name),$couid,$stateid));
			$citid=mysql_insert_id();
			$name="<font color=red>$name</font>";
			++$cit_count;
		}
		else{	
			$citid=mysql_result($sth,0);
		}
//			continue;
		$locid=0;
		$sth=mysql_query(sprintf($sthloc,$yr,$citid));
		if(mysql_num_rows($sth)){
			$locid=mysql_result($sth,0);
			mysql_query(sprintf($sthlocupd,quote_smart($locdata),$locid));
			++$locupd_count;
		}
		else{
			mysql_query(sprintf($sthlocins,$yr,$citid,quote_smart($locdata)));
			$yr="<font color=red>$yr</font>";
			++$locins_count;
		}
#			$sth = my$dbh->prepare(
#				"SELECT cities.id, countries.id FROM cities,countries WHERE cities.country_id=countries.id ".
#				"AND cities.name=\"$name\" AND countries.name=\"$country\"");
		$status.="$citid, $stateid, $couid";
		echo("<tr><td>$name</td><td>$country</td><td>$state</td><td>$yr".
			"</td><td>$txtchk</td><td>$status</td></tr>\n");
	}
	echo("</table><p>Added <b>$cou_count</b> countries, <b>$state_count</b> states, ".
		"<b>$cit_count</b> cities, <b>$locins_count</b> locations.<br>".
		"Updated <b>$locupd_count</b> locations.");
	rm_all($dir);
}

emit_nav2();
	
function upload_error($msg, $dir){
	echo "<h4>Error: $msg</h4>";
 	if(strlen($dir)){
 		rm_all($dir);
 	}
 	emit_nav2();
	exit();
}
?>
