<?php
if(!isset($EXEC)) die("Access restricted");
include_once('mobi/amtools.php');
reject2index();
?>
	<form method="post" action="upload" enctype="multipart/form-data">
		Upload what: 
		<select name="restype">
			<option value="geoyear" selected="selected">Whole year</option>
			<option value="geodata">Geo data</option>
			<option value="tjar">Archive template</option>
			<option value="tjad">Descriptor template</option>
			<option value="demo">Demo</option>
		</select>
		<input name="error_only" type="checkbox" checked="checked">Report errors only</input>
		<br/><br/>
		<input type="file" name="uploaded_file" size="60" value="" style="width:auto"/>
  	<input type="hidden" name="MAX_FILE_SIZE" value="500000" />
		&nbsp;<input type="submit" name="Action" value="Upload"/>
	</form>
<?php	
	if(!isset($_FILES['uploaded_file'])|| !$_FILES['uploaded_file']['name']){
		return;
	}
	else{
		$fname=$_FILES['uploaded_file']['name'];
		
		$restype=$_POST['restype'];

		if(!strlen($restype) || !strlen($fname)){
			echo "error $fname";
			return;
		}
		if(strcmp($restype,'geoyear')==0){
			up_geoyear($fname, 'tbz');
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
	if (!check_ext($fname, $ext))
		return;
	$fn = $_FILES['uploaded_file']['tmp_name'];
	$FF0=fopen($fn,"rb");
	$data=fread($FF0, filesize($fn));
	fclose($FF0);
	if(mysql_query(sprintf("UPDATE source SET data=%s WHERE id=%s", quote_smart($data), $resid))){
		echo "<p>Uploaded successfully.";
	}
	else{
		upload_error("Update error: " . mysql_error());
	}
}
	
function check_ext($fname, $ext){
	$myext=substr($fname,-4);
	if(strcmp($myext,'.'.$ext)/* && strcmp($ext,'.zip')*/){
		upload_error("Invalid archive: '$fname'",'');
		return 0;
	}
	return 1;
}

function up_geoyear($fname, $ext){
	global $DIR_INBOX, $UNTBZ;
	if (!check_ext($fname, $ext)) 
		return;
	$fh = $_FILES['uploaded_file']['tmp_name'];
	list($dir,$fn)=amtools_random(0,'mobi/dl/inbox','');
//	echo "$dir";
	mkdir($dir);
	$cmd=sprintf($UNTBZ, $fh, $dir);
	$res=exec($cmd);
	foreach (glob($dir.'/*', GLOB_ONLYDIR) as $country_dir)
		process_country($country_dir);
	rm_all($dir);
}

function up_geodata($fname, $ext){
	global $DIR_INBOX, $UNZIP;
	if (!check_ext($fname, $ext)) 
		return;
	$fh = $_FILES['uploaded_file']['tmp_name'];
	list($dir,$fn)=amtools_random(0,'mobi/dl/inbox','');
//	echo "$dir";
	mkdir($dir);
	$cmd=sprintf($UNZIP, $fh, $dir);
	$res=exec($cmd);
	process_country($dir);
	rm_all($dir);
}

function process_country($dir){
	$report_errors_only = isset($_POST['error_only']);
	$fn=glob("$dir/*.txt");
	if(count($fn)!=1){
		upload_error("TXT must be exactly one file in archive $dir/*.txt, not ".count($fn), $dir);
	}
	else {
		echo("<br/><b>Reading $fn[0]</b><br><table style='font-size:9pt' border=1><tr><th>City</th>".
			"<th>Country</th><th>State</th><th>Year</th><th>TXT</th><th>Cities DB</th></tr>");
		$sthcou = "SELECT countries.id FROM countries WHERE countries.name=%s";
		$sthcit = "SELECT cities.id FROM cities,countries WHERE cities.name=%s AND country_id=%s AND state_id=%s";
		$sthstate = "SELECT states.id FROM states,countries WHERE states.name=%s AND country_id=%s";
		$sthcouins = "INSERT INTO countries(name,continent) VALUES (%s,%s)";
		$sthstateins = "INSERT INTO states(name,country_id) VALUES (%s,%s)";
		$sthcitins = "INSERT INTO cities(name,country_id,state_id) VALUES (%s,%s,%s)";
		$sthloc = "SELECT id FROM locations WHERE year=%s AND city_id=%s";
		$sthlocupd = "UPDATE locations SET data=0x%s WHERE id=%s";
		$sthlocins = "INSERT INTO locations(year,city_id,data) VALUES(%s,%s,0x%s)";
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
			list($name, $country, $yr, $txtchk, $status, $state, $is_error)=array('','',0,'','','', true);
			$rec=explode('|', $cc);
			if(count($rec)<5){
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
			$curfn=sprintf("$dir/Data%04d.dat",$i++);
			if(!file_exists($curfn)) continue;
			$FF0=fopen($curfn,"rb");
			$tr='';
			$locdata='';
			$yr=fread($FF0,2);
			$yr=current(unpack("n",$yr));
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
				if(strlen($state)){
					$tst.=", $state";
				}
				if(strpos($tst, $len)===false){
					$txtchk="<span class=\"alert\">doesn't match, found <b>$len</b></span>";
				}
				else{
					$txtchk="<b>OK</b>";
					$is_error = false;
				}
			}
			else{
				$txtchk="<span class=\"alert\">missing</span>";
			}
			$couid=0;
			$sth=mysql_query(sprintf($sthcou,quote_smart($country)));
			if(!mysql_num_rows($sth)){
				$que=sprintf($sthcouins,quote_smart($country),quote_smart($continent));
	//			echo $que;
				$sth=mysql_query($que);
				$couid=mysql_insert_id();
				$country="<span class=\"alert\">$country</span>";
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
					$state="<span class=\"alert\">$state</span>";
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
				$name="<span class=\"alert\">$name</span>";
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
				mysql_query(sprintf($sthlocupd,bin2hex($locdata),$locid));
				++$locupd_count;
			}
			else{
				mysql_query(sprintf($sthlocins,$yr,$citid,bin2hex($locdata)));
				$yr="<span class=\"fine\">$yr</span>";
				++$locins_count;
				$locid=mysql_insert_id();
			}
			$sth=mysql_query("SELECT LENGTH(data) FROM locations WHERE id=$locid");
			$datalen=mysql_result($sth,0);
	#			$sth = my$dbh->prepare(
	#				"SELECT cities.id, countries.id FROM cities,countries WHERE cities.country_id=countries.id ".
	#				"AND cities.name=\"$name\" AND countries.name=\"$country\"");
			$status.="$citid, $stateid, $couid";//, #".strlen($locdata)."/$datalen";
			if ($is_error || !$report_errors_only)
				echo("<tr><td>$name</td><td>$country</td><td>$state</td><td>$yr".
					"</td><td>$txtchk</td><td>$status</td></tr>\n");
		}
		echo("</table><p>Added <b>$cou_count</b> countries, <b>$state_count</b> states, ".
			"<b>$cit_count</b> cities, <b>$locins_count</b> locations.<br>".
			"Updated <b>$locupd_count</b> locations.<br/>");
	}
}
	
function upload_error($msg, $dir){
	echo "<h4 class=\"alert\">Error: $msg</h4>";
 	if(strlen($dir)){
 		rm_all($dir);
 	}
}
?>
